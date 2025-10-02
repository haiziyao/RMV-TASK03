// fit_ballistic.cpp
#include <ceres/ceres.h>
#include <ceres/autodiff_cost_function.h>
#include <iostream>
#include <vector>
#include <cmath>
using namespace std;
struct Obs {
  double t;
  double x;
  double y;
};


struct BallisticResidual {
  BallisticResidual(double t, double x_obs, double y_obs, double x0, double y0)
    : t_(t), x_obs_(x_obs), y_obs_(y_obs), x0_(x0), y0_(y0) {}

  template <typename T>
  bool operator()(const T* const vx0, const T* const vy0,
                  const T* const g, const T* const k,
                  T* residuals) const {

    T t = T(t_);
    T exp_term = ceres::exp(-k[0] * t);

    T x_pred = T(x0_) + vx0[0] / k[0] * (T(1.0) - exp_term);
    T y_pred = T(y0_) + (vy0[0] + g[0] / k[0]) / k[0] * (T(1.0) - exp_term) - g[0] / k[0] * t;

    residuals[0] = x_pred - T(x_obs_);
    residuals[1] = y_pred - T(y_obs_);
    return true;
  }

 private:
  double t_, x_obs_, y_obs_, x0_, y0_;
};

int cal(const std::vector<Obs>& obs_in) {
  std::vector<Obs> obs = obs_in;


  double x0 = obs.front().x;
  double y0 = obs.front().y;

  
  const double gmin = 100.0, gmax = 1000.0;
  const double kmin = 0.01, kmax = 1.0;

  
  double t_last = obs.back().t;
  double dx = obs.back().x - x0;
  double dy = obs.back().y - y0;
  double vx0 = dx / std::max(1e-6, t_last);
  double vy0 = dy / std::max(1e-6, t_last);
  double g = 500.0;
  double k = 0.1;

  ceres::Problem problem;
  for (const auto& ob : obs) {
    ceres::CostFunction* cf =
      new ceres::AutoDiffCostFunction<BallisticResidual, 2, 1, 1, 1, 1>(
        new BallisticResidual(ob.t, ob.x, ob.y, x0, y0));
    problem.AddResidualBlock(cf, nullptr, &vx0, &vy0, &g, &k);
  }

  // 直接给 g,k 设上下界
  problem.SetParameterLowerBound(&g, 0, gmin);
  problem.SetParameterUpperBound(&g, 0, gmax);
  problem.SetParameterLowerBound(&k, 0, kmin);
  problem.SetParameterUpperBound(&k, 0, kmax);

  // 求解
  ceres::Solver::Options options;
  options.linear_solver_type = ceres::DENSE_QR;
  options.minimizer_progress_to_stdout = false;
  options.max_num_iterations = 200;
  ceres::Solver::Summary summary;
  ceres::Solve(options, &problem, &summary);
  std::cout << summary.BriefReport() << "\n";

  std::cout << "Estimated vx0 (px/s): " << vx0 << "\n";
  std::cout << "Estimated vy0 (px/s): " << vy0 << "\n";
  std::cout << "Estimated g    (px/s^2): " << g << "\n";
  std::cout << "Estimated k    (1/s): " << k << "\n";

  // 计算 RMSE
  double se_x = 0.0, se_y = 0.0;
  for (const auto& ob : obs) {
    double exp_term = std::exp(-k * ob.t);
    double x_pred = x0 + vx0 / k * (1.0 - exp_term);
    double y_pred = y0 + (vy0 + g / k) / k * (1.0 - exp_term) - g / k * ob.t;
    double dx = x_pred - ob.x;
    double dy = y_pred - ob.y;
    se_x += dx*dx;
    se_y += dy*dy;
  }
  double rmse_x = std::sqrt(se_x / obs.size());
  double rmse_y = std::sqrt(se_y / obs.size());
  std::cout << "RMSE_x (px): " << rmse_x << ", RMSE_y (px): " << rmse_y << "\n";

  return 0;
}
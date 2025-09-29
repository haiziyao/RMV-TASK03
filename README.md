# TASK03 
### 任务流程与结果说明
#### 使用说明
可执行文件在 bin/app  
1.程序开始执行时候，随意按下键可以切换下一张图片，直到截止。    
2.截止后需要按“Esc”才可以退出“视察轮廓模式”  
3.之后便自动计算
#### 结果展示
``` cpp
iter      cost      cost_change  |gradient|   |step|    tr_ratio  tr_radius  ls_iter  iter_time  total_time
   0  4.156861e+07    0.00e+00    1.39e+05   0.00e+00   0.00e+00  1.00e+04        0    2.80e-03    2.85e-03
   1  1.710794e+05    4.14e+07    9.18e+03   5.83e+02   9.96e-01  3.00e+04        1    5.16e-03    8.03e-03
   2  5.513277e+02    1.71e+05    5.43e+01   3.91e+01   1.00e+00  9.00e+04        1    5.30e-03    1.34e-02
   3  5.446944e+02    6.63e+00    3.02e-02   3.54e-01   1.00e+00  2.70e+05        1    5.70e-03    1.91e-02
Ceres Solver Report: Iterations: 4, Initial cost: 4.156861e+07, Final cost: 5.446944e+02, Termination: CONVERGENCE
Estimated vx0 (px/s): 244.43
Estimated vy0 (px/s): 341.186
Estimated g    (px/s^2): 486.865
Estimated k    (1/s): 0.0402198
RMSE_x (px): 1.75352, RMSE_y (px): 2.11905
```

#### 问题与解决
##### 图片轮廓捕捉不到与解决
对于刚开始进行轮廓捕捉，发现效果特别不好。  
通过Trackbar动态调节HSV，发现图片的蓝色块虽然看起来很容易用inRange()处理直接得到maskimg。  
但是实际上H蓝点各部分HSV差异很大  
**所以使用高斯模糊，用比较大的核进行处理，效果显著**  
之后还有一个坑，就是需要用反向二值化处理一下图片不然还是无法找到轮廓
##### 拟合严重偏离与坐标变换
刚开始的时候使用findcontours之后就直接用Moment处理得到中心点坐标了。    
但是带入拟合函数发现g一直往边缘值拟合，直到到达g=100停止拟合，十分难办。  
之后突然想起这个**像素坐标系和拟合坐标系不一样**，所以需要用 y' = H -y  进行转换  
转换之后得到的坐标拟合十分成功。
#### 代码讲解
```
src
├── findhsv.cpp    动态Trackbar函数，得到controus以及坐标函数
├── findhsv.h
├── fit.cpp     拟合优化求解函数
├── fit.h
└── main.cpp
```
``` c++
//findhsv.h
void findhsv();   //含有Trackbar试图找到最适合的HSV
std::vector<Obs> testmain();  //得到中心点坐标的函数
int saveimg();    //进行一些辅助操作
```
``` c++
int cal(vector<Obs> obs);  //函数求解
```
#### 项目目录
```
生成的可执行文件为  bin/app  
根目录下有 build.sh    #可以一键启动编译和执行
```
  
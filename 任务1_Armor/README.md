# RM_armor_identify_test
这是一个Robomaster装甲板简单识别测试
### 主要思路
首先感谢上海交通大学云汉交龙战队于2019年开源的视觉代码，本代码的一些思路和架构很大程度上参考了上交的代码（尽管初看时还不大理解），当然只是练手，简化了很多。<br>
主要是设计了ArmorBox,LightBlob,AutoAim三个类，但还是没有考虑周全，很多操作依赖于opencv中的rotatedrect类实现。<br>
输入视频的一帧后，先用LightBlobidentiy函数找到所有灯条，再用ArmorBoxidentify函数遍历灯条，匹配装甲板，最后绘制装甲板并输出。<br>
##### LightBlobidentify函数
在LightBlobidentify函数中，我先用通道相减的方法提取出要特定颜色（红色/蓝色），然后进行二值化等操作。同时我观察发现，在较低的曝光下，灯条边缘呈现特定颜色而中心仍然过曝，单纯靠通道相减并不能提取出过曝部分。
因此我对原始图片进行一次二值化，目的在于提取出中心的过曝部分，再和通道相减后二值化的图片相加，得到add_image后进行下一步处理。<br>
这样的操作提高了灯条的识别率，但也带来了其他的一些问题，例如有一定几率误识别己方装甲板，可能需要进一步优化emmm。<br>
后面的处理就简单了，计算矩形的长宽比和面积比，基本能够准确识别出灯条。<br>
##### ArmorBoxidentiy函数
在这个函数中，我通过计算旋转矩形的角度差、高度差、间距和长宽比来匹配装甲板，实际测试效果还行。<br>
### 使用方法
ubuntu 18.04 <br>
(在项目文件夹下) <br>
```
mkdir build
cd build
cmake ..
make
cd ../bin
./main
```
### 测试效果
测试环境：ubuntu 18.04<br>处理器: ryzen7 4800u<br>
测试文件：分辨率960×540 原始文件为1080p120fps （深圳跟赛期间拍摄hhhh 可惜没有红色的素材）<br>
测试结果：识别率约为95.3%，帧率在90fps~100fps浮动 <br>
### 后续改进方向：
学习使用多线程加速处理图像(不然白瞎了我这8c16t的处理器)，加入ROI区域，应该可以再把帧率提高一点？<br>
还有卡尔曼滤波，本来也想加上的，然而emmm下次一定。<br>
本代码的鲁棒性也并不是很好，只适用于特定曝光值和gamma的情况，也希望未来可以学习用CNN进行检测和分类 <br>
### 最后
如果真的有人看我的垃圾代码<br>
可以提提修改意见或者帮忙找找BUG<br>
非常感谢！<br>
<完结撒花> <br>


<好吧并没有完结>
### 更新内容
<2021.9.14更新：改进了anglejudge函数，识别率提高到99.3%><br>
<2021.9.15更新：看到招新群里给出的视觉实践任务，把绘制装甲板的方案改了一下，虽然还不是很完美><br>
<2021.9.16更新：感觉这东西没什么人看，就拿来练习git和cmake的使用了，更新了文件组织结构和使用方法> <br>
<2021.9.17更新：增加了一个装甲板判断函数，虽然识别率降到了98.67%，但大幅降低了误识别率(没有精确测试，但比起之前降低了约80%)；同时改了绘制线条的颜色><br>
<2021.9.18更新：新增一个类，将帧率和识别率集中到performance类中，作用嘛......main函数会简洁一点；同时再次修改了绘制方案，可以给出装甲板center坐标><br>
<2021.10.15更新: 时隔一个月后再度更新。识别框架和算法基本没改，加了一个canny边缘检测，识别率有所提高；也试着用闭区域代替两次二值化，但在特定角度效果不理想.
此外增加了反卡尔曼滤波进行预测，并加入了PNP解算(部分轴的角度有正负横跳的情况，暂时还没有搞清楚原因emmm> <br>

# RMV_TASK04-1-
## 参考教程
[教程一](https://blog.csdn.net/weixin_43871310/article/details/105584650/?ops_request_misc=&request_id=&biz_id=102&utm_term=opencv%E5%9B%BE%E5%83%8F%E8%AF%86%E5%88%ABRM%E8%A3%85%E7%94%B2%E6%9D%BF&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-3-105584650.142^v100^pc_search_result_base2&spm=1018.2226.3001.4187)，[教程二](https://blog.csdn.net/weixin_45685193/article/details/123112639?ops_request_misc=&request_id=&biz_id=102&utm_term=opencv%E5%9B%BE%E5%83%8F%E8%AF%86%E5%88%ABRM%E8%A3%85%E7%94%B2%E6%9D%BF&utm_medium=distribute.pc_search_result.none-task-blog-2~all~sobaiduweb~default-6-123112639.142^v100^pc_search_result_base2&spm=1018.2226.3001.4187)，[教程三](https://blog.csdn.net/qq_40403096/article/details/107039702?ops_request_misc=%257B%2522request%255Fid%2522%253A%25220D9CE729-0C0D-4A65-8100-6E3C623D7FE7%2522%252C%2522scm%2522%253A%252220140713.130102334.pc%255Fall.%2522%257D&request_id=0D9CE729-0C0D-4A65-8100-6E3C623D7FE7&biz_id=0&utm_medium=distribute.pc_search_result.none-task-blog-2~all~first_rank_ecpm_v1~rank_v31_ecpm-4-107039702-null-null.142^v100^pc_search_result_base2&utm_term=opencv%E5%9B%BE%E5%83%8F%E8%AF%86%E5%88%ABRM%E8%A3%85%E7%94%B2%E6%9D%BF&spm=1018.2226.3001.4187)，[教程四](https://blog.csdn.net/MobooV/article/details/128730417)

其中代码架构参考教程三，相机内参参考教程四
## 存在问题与解决方案
视频中有三个蓝色灯条识别的间距、高度、角度都十分相似，识别的中心点极易在两两间跳跃。尝试过在两两灯条间识别数字，但识别效果并不好。于是根据其最小拟合旋转矩形minRect设置面积阈值大小，筛掉了那个不符合要求的灯条，使中心点更稳定
## 结果
视频私聊发过了，截图是终端输出结果

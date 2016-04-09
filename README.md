# 重大声明 #
  * 感谢大家的关心，Gypsophila希望为更多的清华毕业生带去最后的一丝回忆。
  * 由于毕业很久，已经没有网络学堂账号，无法为该项目更新，程序难免出现各种崩溃情形，即将做出版本更新（保留核心功能，去除课堂讨论功能）。
  * 尽快将代码整理，将之公布。
  * 欢迎有志维护代码的同学加入。
> `2013年12月4日`

---

## Introduction to Gypsophila ##
Gypsophila is a library for web course at Tsinghua University written in C programming language. <br />
The goal for developing gypsophila is to provide a library for login and mirror web course at Tsinghua University.<br />
There are many applications bulit on gypsophila, including different platforms(Windows, Linux, MacOSX, etc).<br />

## Gypsophila简介 ##
  * Gypsophila是一个提供登录、下载清华大学网络学堂各种学习资源的_C_ 函数库。由于毕业生离校只能够使用一年的网络学堂的使用权限，因此Gypsophila开发的初衷是为了清华大学毕业生们能够方便镜像清华大学网络学堂的所有与自己有关的内容。<br>
<ul><li>Gypsophila之所以取名“满天星”，是寓意清华大学的毕业生们能够“聚是一团火，散是满天星。”<br>
</li><li>现在有许多应用程序基于Gypsophila开发，这些应用程序运行在各种平台上，包括Windows、Linux、MacOSX。</li></ul>

<h2>Gypsophila目前的问题及措施</h2>
2013年5月7日更新：<br>
谢各位对于项目的关注。Gypsophila目前的版本的确存在镜像课程讨论时崩溃的问题，我推测原因可能是网络学堂改版造成的。现在的网络学堂与三年前相比肯定做出了更多的改进，程序出现了无法适配的问题。<br>
<br>
我可能近期修改或者将源代码发布，更大的可能性是源代码发布。我更希望有意愿的清华学生维护本程序。<br>
<br>
<h2>Gypsophila提供的函数可以实现如下功能</h2>
<ul><li>登录<a href='http://learn.tsinghua.edu.cn'>清华大学网络学堂</a>
<ol><li>毕业生请使用学号登录<br>
</li><li>非毕业生同学请使用学号或者字母类型的账号登录均可</li></ol></li></ul>

<ul><li>镜像各种课程，包括<br>
<ol><li>现在正在学习的课程；<br>
</li><li>下学期课程；<br>
</li><li>以前的课程<br>
</li></ol></li><li>对于每一门课程，Gypsophila提供函数可以镜像如下内容：<br>
<ol><li>课程公告：包含教师与助教的所有公告以及所有内容<br>
</li><li>课程文件：按照教师对于课程文件的分类进行分目录下载，并（即将）提供批处理的个性化命名方式；<br>
</li><li>课程作业：包含教师提供的所有课程作业，布置题目、附件，同时包含学生自己作业、教师批阅得分、教师评语等所有内容；<br>
</li><li>课程讨论：包含所有的同学讨论并且以xml格式组织起来，同时下载所有的课程讨论附件<br>
<h2>下载目录</h2>
Gypsophila正在完善中，因此下载的文件内容在当前目录的Course文件夹内，请多配置文件敬请期待。</li></ol></li></ul>

<h2>Bugs</h2>
反应反馈各种bugs。<br>
<br>
<h2>FAQ</h2>
1、Windows 7版本提示缺少ssleay32.dll，应该怎么办？<br>
<br>
答复：请参考Windows XP版本，里面包含了所有的必须的dll文件，缺什么dll，就从XP版本复制该文件到程序目录下即可。<br>
一般同学的电脑是不缺这些文件的，缺少这些文件原因很多，可能是某些电脑常用插件没有安装导致的。<br>
<br>
2、为什么有些时候网络学堂密码错了，也告诉我登录成功？<br>
<br>
答复：已知bug，因为我没有对你的密码做任何判断。如果你的密码是对的，那么程序正常工作，备份你的所有课程；如果密码错了，那么程序不做任何备份。不过这个提示的确有问题。<br>
<br>
3、为什么我的密码正确，但是无法下载课程？<br>
<br>
答复：当前网络学堂网络不可用，请去自己登录一下网络学堂网站看一下，估计登录不上去。如果可以登上去，而我的程序登不上去的话，请给我留言，我将会解决。

# Cloud-storage-system-based-on-private-protocol
基于私有协议的云存储系统
开发环境：Linux 
开发工具：Vim,Gcc,Gdb,Makefile脚本,Shell脚本,Mysql,NPTL线程库
项目描述：参照百度网盘功能仿写的项目，可实现多用户上传下载，共享数据。服务器端使用线程池模型实现多用户链接
，通过epoll 多路复用相应客户端请求。客户端使用线程词实现长短命令分离，多个上传下载任务并发进行

功能描述：
          基于Openssl 签名防伪造
          RSA非对称加密传输数据
          秒传
          

         服务器端：
               基于线程池，通过Mysql实现的虚拟文件系统
               MD5值差错校验
               文件断点续传
               服务器已有的文件实现秒传
               长短命令分离
               多个上传下载任务并发进行
               客户端超时自动断开
               客户端操作记入log         
         客户端 ：
               sigin 注册
               login 登陆
               mkdir 创建文件夹
               rmdir 删除文件夹
               gets 下载
               puts 上传
               ls 显示当前文件夹下的文件/目录
               cd 支持 cd ../   cd ../../   cd test/
               remove删除文件
               exit 退出
               share 共享文件  
               
        


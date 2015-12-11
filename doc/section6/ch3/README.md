### 网络通信设计与实现

分析五子棋游戏需求后, 网络传输的数据有如下3种类型:
- 游戏中棋子在棋盘中的坐标

- 游戏中的控制信息, 如"和棋"

- 其他扩展信息。

为了能适应着3种不同信息的传输, 所以在游戏的网络传输中, 必须加入一个数据包结构。

其数据域格式如下所示:

数据长度 | 消息类型 | 数据
---|---|---
2字节 | 1字节 | n字节
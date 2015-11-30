

1.直接操作洛哈数据库

2.内部版需定义 #define  YQC_TECH  相反外部版不需要取消定义//#define  YQC_TECH

3.内部版以serial_id为主

4.外部版不操作serial_id,mid或sid代替

5.actor表的serial_id不能为空，外部版默认serial_id=0  media的serial_id为空
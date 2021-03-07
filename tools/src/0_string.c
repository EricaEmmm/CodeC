/*
length
    char dst[10];               // sizeof(dst)=10, strlen(dst)=0
    char *src = "0123456789";   // sizeof(src)=4, strlen(src)=10
    char src1[] = "0123456789"; // sizeof(src)=11, strlen(src)=10
memory
    memcpy(dst, src, strlen(dst));          // memcpy会完整的复制num个字节，不会因为遇到'\0'而结束
    strcpy(dst, src);                       // src所指的有'\0'结尾的字符串复制到dst，但dst后的空间会被覆盖
    strncpy(dst, src, strlen(dst));         // 拷贝src前n个字符,不会追加'\0'
    memset(dst, 0, strlen(dst)+1);
    
    memcpy_s(dst,  strlen(dst)+1, src, 11);     // 失败，要拷贝内存的大小11>dst的实际大小10（11改10可成功）
    strcpy_s(dst, strlen(dst)+1, src);          // 失败，strlen(src)比destMax-1大；执行后自动在字符串末尾补充\0
    memset_s(dst, strlen(dst)+1, 0, count);     // dst为NULL或count>destMax，则失败
cat
    char buffer[NUM];
    int len = sprintf(buffer, "%d, ", 2);
    len += sprintf(buffer + len, "%s-%s", "a", "b");
    int ret = strcat(buffer, "abc");
    ret = strcat(buffer + 3, "1234");
    
    len = sprintf_s(buffer, NUM, "%d, ", 2);
    len += sprintf_s(buffer + len, NUM - len, "%s-%s", "a", "b");   // 字符串长度比destMax-1大，则失败
    ret = strcat_s(buffer, NUM, "abc");                             // 字符串长度比destMax-1大，则失败
    ret = strcat_s(buffer + 3, NUM - 3, "1234");
switch
    int num1 = atoi("123");
    int num2 = atoi("-123");
    char str1[10];
    char str2[10];
    itoa(1234, str1, 10);
    itoa(-1234, str2, 10);
    printf("%s, %s\n", str1, str2);

*/
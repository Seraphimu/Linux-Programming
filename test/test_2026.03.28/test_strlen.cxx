#include <iostream>
#include <cstring>
//看看strlen和sizeof在极限情况下的输出
int main(void) {
	char str[6] = "Hello";

	//C会把字符串后添加一个'\0'作为结尾标识符
	std::cout << "str: " << str << std::endl;
	//输出的结果是6, 包括'\0'
	std::cout << "sizeof str: " << sizeof str << std::endl;
	//输出的结果是5, 只包括有效字符
	std::cout << "strlen(str): " << strlen(str) << std::endl;
	

	char empty_str[1024];
	//输出1024, 不管有没有有效字符, 内存空间开辟多大, 它就会返回多少个字节
	std::cout << "sizeof empty_str: " << sizeof empty_str << std::endl;
	//输出0, 只计算有效字符大小
	std::cout << "strlen empty_str: " << strlen(empty_str) << std::endl;



	return 0;
}

a.out: Test.cpp
# 巧记：编译默认的文件是a.out, 而依赖文件的是$@ 与a极为接近
	g++ -g $^ -o $@

.PHONY: clean
clean:
	rm -f a.out

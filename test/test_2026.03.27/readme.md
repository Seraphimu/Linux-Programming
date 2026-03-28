所谓的`file_struct`在哪里?

在 `task_struct` 下:

```c
struct task_struct {
	//...
	struct files_struct *files;
	//...
};
```

`files_struct` 下:

```c
struct files_struct {
	//...
	struct file ** fd;      /* current fd array */
	struct file * fd_array[NR_OPEN_DEFAULT];
	//...
};
```

`file` 下:

```c
struct file {
	//...
	//mode_t == unsigned int
	mode_t			f_mode;
	//loff_t == long long
	loff_t			f_pos;
	//文件所属者和组
	unsigned int		f_uid, f_gid;
	//...
};
```



所谓的fd不是 `struct file` 中的一个字段, 而是文件打开后, `struct files_struct` 下的 `fd_array` 的下标!

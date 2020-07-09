.text

.data
message:	 .ascii "Hello World!\n"
endmsg:
buff:	.fill 1024,1,0

.globl _start

_start:

/* simple read(1, buff, 1024) */

	mov $3, %eax
	mov $1, %ebx
	mov $buff, %ecx
	mov $1024, %edx
	int $0x80

/* simple write(1, message, strlen(message)) */

	mov $4, %eax
	mov $1, %ebx
	mov $message, %ecx
	mov $(endmsg-message), %edx
	int $0x80

/* _exit */

	mov $1, %eax
	mov $77, %ebx
	int $0x80

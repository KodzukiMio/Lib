; RCX,RDX,R8,R9,stack
extern _cout_str:proc
extern _cout_i64:proc
extern _cout_c:proc
extern _cin_i64:proc
extern _cin_c:proc
.code
set_rax proc
	mov rax,rcx
	ret
set_rax endp
set_rbx proc
	mov rbx, rcx
	ret
set_rbx endp
set_rdx proc
	mov rdx, rcx
	ret
set_rdx endp
set_rsi proc
	mov rsi, rcx
	ret
set_rsi endp
set_rdi proc
	mov rdi, rcx
	ret
set_rdi endp
set_rsp proc
	mov rsp, rcx
	ret
set_rsp endp
set_rbp proc
	mov rbp, rcx
	ret
set_rbp endp
get_rax proc
	ret
get_rax endp
get_rbx proc
	mov rax, rbx
	ret
get_rbx endp
get_rsi proc
	mov rax, rsi
	ret
get_rsi endp
get_rdi proc
	mov rax, rdi
	ret
get_rdi endp
get_rsp proc
	mov rax, rsp
	ret
get_rsp endp
get_rbp proc 
	mov rax, rbp
	ret
get_rbp endp

swap proc;p64* L,p64* R
	push rbx
	push rax
	mov rax,[rcx]
	mov rbx,[rdx]
	mov [rcx],rbx
	mov [rdx],rax
	pop rax
	pop rbx
	ret
swap endp

_sumof64 proc;u64 length,i64* address,u64 offset
	xor rax,rax
L19:
	add rax,[rdx]
	add rdx,r8
	dec rcx
	jnz L19
	ret
_sumof64 endp

_sumof32 proc;u64 length,i32* address,u64 offset
	xor rax,rax
L31:
	add eax,[rdx]
	add rdx,r8
	dec rcx
	jnz L31;if not zero
	ret
_sumof32 endp

check_parity proc;int64_t val ->rcx
	xor rax,rax
	test rcx,1
	setnz al
	ret	
check_parity endp

.data
ostr95 byte "hello world !",0
ostr99 byte "bad world !",0
.code
;这个_test_main是学习用的.
;读取2个用户输入的数字,相同输出hello world,不同输出bad world
_test_main proc export frame
	.endprolog
	sub rsp,28h
	;call _test_main时,返回地址8bytes被压入栈,|sub rsp,8h|进行16字节对齐
	;sub rsp,20h,保留32字节(选择保存rcx,rdx,r8,r9)
	mov [rsp+8],rax;保存2reg,还有16字节
	mov [rsp+16],rbx
	;dosomething...
	;读取数字
	call _cin_i64
	mov rbx,rax
	call _cin_i64
	mov rdx,rax
	;比较rbx == rdx
	cmp rbx,rdx
	jz L109;true => L109
	;jz是ZF = 1跳转,jnz是ZF = 0跳转 
	;=> | jnz -> if(val != 0) | jz -> if(val == 0) |
	mov rbx,0;false
	jmp L110
L109:
	mov rbx,1
L110:
	cmp rbx,1
	jz L136 
	lea rcx,[ostr99]
	jmp L137
L136:
	lea rcx,[ostr95]
L137:
	call _cout_str
	mov rax,[rsp+8];恢复2reg
	mov rbx,[rsp+16]
	add rsp,28h;恢复
	ret
_test_main endp

end
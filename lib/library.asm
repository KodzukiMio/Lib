; RCX,RDX,R8,R9,stack
.code
set_rax proc export frame
.endprolog
	mov rax,rcx
	ret
set_rax endp
set_rbx proc export frame
.endprolog
	mov rbx, rcx
	ret
set_rbx endp
set_rdx proc export frame
.endprolog
	mov rdx, rcx
	ret
set_rdx endp
set_rsi proc export frame
.endprolog
	mov rsi, rcx
	ret
set_rsi endp
set_rdi proc export frame
.endprolog
	mov rdi, rcx
	ret
set_rdi endp
set_rsp proc export frame
.endprolog
	mov rsp, rcx
	ret
set_rsp endp
set_rbp proc export frame
.endprolog
	mov rbp, rcx
	ret
set_rbp endp
get_rax proc export frame
.endprolog
	ret
get_rax endp
get_rbx proc export frame
.endprolog
	mov rax, rbx
	ret
get_rbx endp
get_rsi proc export frame
.endprolog
	mov rax, rsi
	ret
get_rsi endp
get_rdi proc export frame
.endprolog
	mov rax, rdi
	ret
get_rdi endp
get_rsp proc export frame
.endprolog
	mov rax, rsp
	ret
get_rsp endp
get_rbp proc export frame
.endprolog
	mov rax, rbp
	ret
get_rbp endp

swap proc export frame;p64* L,p64* R
	.endprolog
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

_sumof64 proc export frame;u64 length,i64* address,u64 offset
	.endprolog
	xor rax,rax
L19:
	add rax,[rdx]
	add rdx,r8
	dec rcx
	jnz L19
	ret
_sumof64 endp

_sumof32 proc export frame;u64 length,i32* address,u64 offset
	.endprolog
	xor rax,rax
L31:
	add eax,[rdx]
	add rdx,r8
	dec rcx
	jnz L31
	ret
_sumof32 endp

end
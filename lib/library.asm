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
;State
s101_start equ 0
s101_L_sign equ 1
s101_R_sign equ 2
s101_normal equ 3
s101_success equ 4
s101_invalid equ 5
;sign-table
L_Sign equ '<'
R_Sign equ '>'
R_Next equ ':'
.code
_transition proc
;int current -> rcx,char itrc -> rdx,size_t idx -> r8,size_t imax -> r9
	cmp rcx,s101_start
	jz lsign
	cmp rcx,s101_L_sign
	jz lsign
	cmp rcx,s101_R_sign
	jz rsign
	cmp rcx,s101_normal
	jz normal
	jmp default
lsign:
	cmp rdx,L_Sign
	jz L127
	jmp default
L127:
	cmp r8,0
	jz L130
	mov rax,s101_normal
	ret
	L130:
	mov rax,s101_L_sign
	ret
normal:
	cmp rdx,R_Sign
	jz L146
	cmp rdx,L_Sign
	jz default
	cmp rdx,R_Next
	jz default
	jmp L160
rsign:
	cmp r8,r9
	jz L155
	cmp rdx,R_Next
	jz L160
default:
	mov rax,s101_invalid 
end133:
	ret
L146:
	mov rax,s101_R_sign
	ret
L155:
	mov rax,s101_success
	ret
L160:
	mov rax,s101_normal
	ret
_transition endp

_verify_token proc;<<key>:value>
;const char*str->rcx,size_t length->rdx
	push r10
	push r8
	push r9
	push r13
	mov r10,rcx;char*
	mov r9,rdx;length
	xor r8,r8;idx=0
	mov r13,s101_start;state
	mov rax,r13
L171:
	mov rcx,r13
	movzx rdx,byte ptr[r10+r8]
	call _transition
	mov r13,rax
	cmp r13,s101_success
	jz L178
	cmp r13,s101_invalid
	jz L180
	cmp r8,r9
	inc r8
	jbe L171
	mov rax,0
cl184:
	pop r13
	pop r9
	pop r8
	pop r10
	ret
L178:
	mov rax,1
	jmp cl184
L180:
	mov rax,0
	jmp cl184
_verify_token endp

safe_call proc uses rbp rbx rcx rdx rsi rdi r8 r9 r10 r11 r12 r13 r14 r15
;rax 作为参数,保证不改变除了(EFL,RAX,RIP)其它reg的值.不支持栈传递参数
	sub rsp,32
	push r15
	mov [rsp+8],r15
	mov [rsp+16],rsp
	mov [rsp+24],r11
	mov [rsp+32],rbp
	call rax
	mov rbp,[rsp+32]
	mov r11,[rsp+24]
	mov rsp,[rsp+16]
	mov r15,[rsp+8]
	pop r15
	add rsp,32
	ret
safe_call endp
cin_i64 proc
	mov rax,offset _cin_i64
	call safe_call
	ret
cin_i64 endp
cin_c proc
	mov rax,offset _cin_c
	call safe_call
	ret
cin_c endp
cout_i64 proc
	mov rax,offset _cout_i64
	call safe_call
	ret
cout_i64 endp
cout_str proc
	mov rax,offset _cout_str
	call safe_call
	ret
cout_str endp
cout_c proc
	mov rax,offset _cout_c
	call safe_call
	ret
cout_c endp
.data
fp244 real8 3.1415926e+0
.code
test_main proc;
	

	ret
test_main endp
safe_main proc export frame
	.endprolog
	push rax
	mov rax,offset test_main
	call safe_call
	pop rax
	ret
safe_main endp

end
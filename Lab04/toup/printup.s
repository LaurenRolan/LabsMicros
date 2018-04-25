/*
  printup.s: Assembly function converting a string to upper case.

  Copyright (c) 2016 Walter Fetter Lages <w.fetter@ieee.org>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

    You can also obtain a copy of the GNU General Public License
    at <http://www.gnu.org/licenses>.
*/

	.intel_syntax noprefix

	.text
    	.global  printup

printup:
	push	ebp
	mov	ebp,esp
	push	ebx
	push	edi
	push	esi

	mov	edi,[ebp+8]
	mov	esi,edi
	xor	ecx,ecx

	mov	ah,'A
	xor	ah,'a
	not	ah
	cld
L1:	lods	al,byte ptr[esi]
	and	al,ah
	jz	strend
	stos	byte ptr[edi],al
	inc	ecx
	jmp	L1

strend:

	push	ecx		# message length
	push	[ebp+8]		# pointer to message to write

	call	print
	add	sp,8

	pop	esi
	pop	edi
	pop	ebx
	pop	ebp
	ret

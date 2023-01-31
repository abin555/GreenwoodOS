
tetris.elf:     file format elf32-i386


Disassembly of section .text:

00000000 <main>:
   0:	8d 4c 24 04          	lea    0x4(%esp),%ecx
   4:	83 e4 f0             	and    $0xfffffff0,%esp
   7:	ff 71 fc             	pushl  -0x4(%ecx)
   a:	55                   	push   %ebp
   b:	89 e5                	mov    %esp,%ebp
   d:	53                   	push   %ebx
   e:	51                   	push   %ecx
   f:	e8 9c 05 00 00       	call   5b0 <__x86.get_pc_thunk.bx>
  14:	81 c3 cc 05 00 00    	add    $0x5cc,%ebx
  1a:	83 ec 0c             	sub    $0xc,%esp
  1d:	8d 83 74 ff ff ff    	lea    -0x8c(%ebx),%eax
  23:	50                   	push   %eax
  24:	e8 c3 05 00 00       	call   5ec <print>
  29:	83 c4 10             	add    $0x10,%esp
  2c:	e8 3c 08 00 00       	call   86d <get_keycode_next>
  31:	83 ec 04             	sub    $0x4,%esp
  34:	c7 c0 20 0a 00 00    	mov    $0xa20,%eax
  3a:	50                   	push   %eax
  3b:	68 90 01 00 00       	push   $0x190
  40:	68 58 02 00 00       	push   $0x258
  45:	e8 53 09 00 00       	call   99d <open_window>
  4a:	83 c4 10             	add    $0x10,%esp
  4d:	c7 c2 04 0a 00 00    	mov    $0xa04,%edx
  53:	89 02                	mov    %eax,(%edx)
  55:	c7 c0 04 0a 00 00    	mov    $0xa04,%eax
  5b:	8b 00                	mov    (%eax),%eax
  5d:	83 ec 08             	sub    $0x8,%esp
  60:	8d 93 b5 fc ff ff    	lea    -0x34b(%ebx),%edx
  66:	52                   	push   %edx
  67:	50                   	push   %eax
  68:	e8 6f 09 00 00       	call   9dc <add_window_event_handler>
  6d:	83 c4 10             	add    $0x10,%esp
  70:	c6 83 00 fd ff ff 01 	movb   $0x1,-0x300(%ebx)
  77:	83 ec 0c             	sub    $0xc,%esp
  7a:	6a 00                	push   $0x0
  7c:	e8 ac 00 00 00       	call   12d <clear_window>
  81:	83 c4 10             	add    $0x10,%esp
  84:	83 ec 0c             	sub    $0xc,%esp
  87:	8d 83 81 ff ff ff    	lea    -0x7f(%ebx),%eax
  8d:	50                   	push   %eax
  8e:	e8 59 05 00 00       	call   5ec <print>
  93:	83 c4 10             	add    $0x10,%esp
  96:	83 ec 04             	sub    $0x4,%esp
  99:	68 ff b6 00 00       	push   $0xb6ff
  9e:	6a 32                	push   $0x32
  a0:	6a 32                	push   $0x32
  a2:	e8 c4 00 00 00       	call   16b <draw_block>
  a7:	83 c4 10             	add    $0x10,%esp
  aa:	e8 5b 01 00 00       	call   20a <draw_title>
  af:	eb 3c                	jmp    ed <main+0xed>
  b1:	e8 b7 07 00 00       	call   86d <get_keycode_next>
  b6:	89 c2                	mov    %eax,%edx
  b8:	c7 c0 00 0a 00 00    	mov    $0xa00,%eax
  be:	88 10                	mov    %dl,(%eax)
  c0:	c7 c0 00 0a 00 00    	mov    $0xa00,%eax
  c6:	0f b6 00             	movzbl (%eax),%eax
  c9:	0f b6 c0             	movzbl %al,%eax
  cc:	3d 81 00 00 00       	cmp    $0x81,%eax
  d1:	75 1a                	jne    ed <main+0xed>
  d3:	c6 83 00 fd ff ff 00 	movb   $0x0,-0x300(%ebx)
  da:	83 ec 0c             	sub    $0xc,%esp
  dd:	8d 83 95 ff ff ff    	lea    -0x6b(%ebx),%eax
  e3:	50                   	push   %eax
  e4:	e8 03 05 00 00       	call   5ec <print>
  e9:	83 c4 10             	add    $0x10,%esp
  ec:	90                   	nop
  ed:	0f b6 83 00 fd ff ff 	movzbl -0x300(%ebx),%eax
  f4:	84 c0                	test   %al,%al
  f6:	75 b9                	jne    b1 <main+0xb1>
  f8:	c7 c0 04 0a 00 00    	mov    $0xa04,%eax
  fe:	8b 00                	mov    (%eax),%eax
 100:	83 ec 0c             	sub    $0xc,%esp
 103:	50                   	push   %eax
 104:	e8 b7 08 00 00       	call   9c0 <close_window>
 109:	83 c4 10             	add    $0x10,%esp
 10c:	83 ec 0c             	sub    $0xc,%esp
 10f:	8d 83 a5 ff ff ff    	lea    -0x5b(%ebx),%eax
 115:	50                   	push   %eax
 116:	e8 d1 04 00 00       	call   5ec <print>
 11b:	83 c4 10             	add    $0x10,%esp
 11e:	b8 00 00 00 00       	mov    $0x0,%eax
 123:	8d 65 f8             	lea    -0x8(%ebp),%esp
 126:	59                   	pop    %ecx
 127:	5b                   	pop    %ebx
 128:	5d                   	pop    %ebp
 129:	8d 61 fc             	lea    -0x4(%ecx),%esp
 12c:	c3                   	ret    

0000012d <clear_window>:
 12d:	55                   	push   %ebp
 12e:	89 e5                	mov    %esp,%ebp
 130:	53                   	push   %ebx
 131:	83 ec 10             	sub    $0x10,%esp
 134:	e8 6f 04 00 00       	call   5a8 <__x86.get_pc_thunk.ax>
 139:	05 a7 04 00 00       	add    $0x4a7,%eax
 13e:	c7 45 f8 00 00 00 00 	movl   $0x0,-0x8(%ebp)
 145:	eb 13                	jmp    15a <clear_window+0x2d>
 147:	c7 c2 20 0a 00 00    	mov    $0xa20,%edx
 14d:	8b 4d f8             	mov    -0x8(%ebp),%ecx
 150:	8b 5d 08             	mov    0x8(%ebp),%ebx
 153:	89 1c 8a             	mov    %ebx,(%edx,%ecx,4)
 156:	83 45 f8 01          	addl   $0x1,-0x8(%ebp)
 15a:	81 7d f8 7f a9 03 00 	cmpl   $0x3a97f,-0x8(%ebp)
 161:	76 e4                	jbe    147 <clear_window+0x1a>
 163:	90                   	nop
 164:	90                   	nop
 165:	83 c4 10             	add    $0x10,%esp
 168:	5b                   	pop    %ebx
 169:	5d                   	pop    %ebp
 16a:	c3                   	ret    

0000016b <draw_block>:
 16b:	55                   	push   %ebp
 16c:	89 e5                	mov    %esp,%ebp
 16e:	56                   	push   %esi
 16f:	53                   	push   %ebx
 170:	83 ec 10             	sub    $0x10,%esp
 173:	e8 34 04 00 00       	call   5ac <__x86.get_pc_thunk.dx>
 178:	81 c2 68 04 00 00    	add    $0x468,%edx
 17e:	c7 45 f4 00 00 00 00 	movl   $0x0,-0xc(%ebp)
 185:	eb 74                	jmp    1fb <draw_block+0x90>
 187:	c7 45 f0 00 00 00 00 	movl   $0x0,-0x10(%ebp)
 18e:	eb 61                	jmp    1f1 <draw_block+0x86>
 190:	8b 4d f4             	mov    -0xc(%ebp),%ecx
 193:	89 c8                	mov    %ecx,%eax
 195:	01 c0                	add    %eax,%eax
 197:	01 c8                	add    %ecx,%eax
 199:	c1 e0 02             	shl    $0x2,%eax
 19c:	8b 4d f0             	mov    -0x10(%ebp),%ecx
 19f:	01 c8                	add    %ecx,%eax
 1a1:	8b 84 82 20 fd ff ff 	mov    -0x2e0(%edx,%eax,4),%eax
 1a8:	83 f8 0f             	cmp    $0xf,%eax
 1ab:	74 1a                	je     1c7 <draw_block+0x5c>
 1ad:	8b 4d f4             	mov    -0xc(%ebp),%ecx
 1b0:	89 c8                	mov    %ecx,%eax
 1b2:	01 c0                	add    %eax,%eax
 1b4:	01 c8                	add    %ecx,%eax
 1b6:	c1 e0 02             	shl    $0x2,%eax
 1b9:	8b 4d f0             	mov    -0x10(%ebp),%ecx
 1bc:	01 c8                	add    %ecx,%eax
 1be:	8b 84 82 20 fd ff ff 	mov    -0x2e0(%edx,%eax,4),%eax
 1c5:	eb 03                	jmp    1ca <draw_block+0x5f>
 1c7:	8b 45 10             	mov    0x10(%ebp),%eax
 1ca:	8b 5d 08             	mov    0x8(%ebp),%ebx
 1cd:	8b 4d f0             	mov    -0x10(%ebp),%ecx
 1d0:	8d 34 0b             	lea    (%ebx,%ecx,1),%esi
 1d3:	8b 5d 0c             	mov    0xc(%ebp),%ebx
 1d6:	8b 4d f4             	mov    -0xc(%ebp),%ecx
 1d9:	01 d9                	add    %ebx,%ecx
 1db:	69 c9 58 02 00 00    	imul   $0x258,%ecx,%ecx
 1e1:	8d 1c 0e             	lea    (%esi,%ecx,1),%ebx
 1e4:	c7 c1 20 0a 00 00    	mov    $0xa20,%ecx
 1ea:	89 04 99             	mov    %eax,(%ecx,%ebx,4)
 1ed:	83 45 f0 01          	addl   $0x1,-0x10(%ebp)
 1f1:	83 7d f0 0b          	cmpl   $0xb,-0x10(%ebp)
 1f5:	76 99                	jbe    190 <draw_block+0x25>
 1f7:	83 45 f4 01          	addl   $0x1,-0xc(%ebp)
 1fb:	83 7d f4 0b          	cmpl   $0xb,-0xc(%ebp)
 1ff:	76 86                	jbe    187 <draw_block+0x1c>
 201:	90                   	nop
 202:	90                   	nop
 203:	83 c4 10             	add    $0x10,%esp
 206:	5b                   	pop    %ebx
 207:	5e                   	pop    %esi
 208:	5d                   	pop    %ebp
 209:	c3                   	ret    

0000020a <draw_title>:
 20a:	55                   	push   %ebp
 20b:	89 e5                	mov    %esp,%ebp
 20d:	56                   	push   %esi
 20e:	53                   	push   %ebx
 20f:	83 ec 10             	sub    $0x10,%esp
 212:	e8 99 03 00 00       	call   5b0 <__x86.get_pc_thunk.bx>
 217:	81 c3 c9 03 00 00    	add    $0x3c9,%ebx
 21d:	c7 45 f4 00 00 00 00 	movl   $0x0,-0xc(%ebp)
 224:	eb 60                	jmp    286 <draw_title+0x7c>
 226:	c7 45 f0 00 00 00 00 	movl   $0x0,-0x10(%ebp)
 22d:	eb 4d                	jmp    27c <draw_title+0x72>
 22f:	8b 45 f4             	mov    -0xc(%ebp),%eax
 232:	8b 94 83 60 ff ff ff 	mov    -0xa0(%ebx,%eax,4),%edx
 239:	8b 45 f0             	mov    -0x10(%ebp),%eax
 23c:	89 c1                	mov    %eax,%ecx
 23e:	d3 ea                	shr    %cl,%edx
 240:	89 d0                	mov    %edx,%eax
 242:	83 e0 01             	and    $0x1,%eax
 245:	85 c0                	test   %eax,%eax
 247:	74 2f                	je     278 <draw_title+0x6e>
 249:	8b 45 f4             	mov    -0xc(%ebp),%eax
 24c:	8b 8c 83 04 fd ff ff 	mov    -0x2fc(%ebx,%eax,4),%ecx
 253:	8b 55 f4             	mov    -0xc(%ebp),%edx
 256:	89 d0                	mov    %edx,%eax
 258:	01 c0                	add    %eax,%eax
 25a:	01 d0                	add    %edx,%eax
 25c:	c1 e0 02             	shl    $0x2,%eax
 25f:	89 c6                	mov    %eax,%esi
 261:	8b 55 f0             	mov    -0x10(%ebp),%edx
 264:	89 d0                	mov    %edx,%eax
 266:	01 c0                	add    %eax,%eax
 268:	01 d0                	add    %edx,%eax
 26a:	c1 e0 02             	shl    $0x2,%eax
 26d:	51                   	push   %ecx
 26e:	56                   	push   %esi
 26f:	50                   	push   %eax
 270:	e8 f6 fe ff ff       	call   16b <draw_block>
 275:	83 c4 0c             	add    $0xc,%esp
 278:	83 45 f0 01          	addl   $0x1,-0x10(%ebp)
 27c:	83 7d f0 1f          	cmpl   $0x1f,-0x10(%ebp)
 280:	7e ad                	jle    22f <draw_title+0x25>
 282:	83 45 f4 01          	addl   $0x1,-0xc(%ebp)
 286:	83 7d f4 04          	cmpl   $0x4,-0xc(%ebp)
 28a:	7e 9a                	jle    226 <draw_title+0x1c>
 28c:	90                   	nop
 28d:	90                   	nop
 28e:	8d 65 f8             	lea    -0x8(%ebp),%esp
 291:	5b                   	pop    %ebx
 292:	5e                   	pop    %esi
 293:	5d                   	pop    %ebp
 294:	c3                   	ret    

00000295 <window_event_handler>:
 295:	55                   	push   %ebp
 296:	89 e5                	mov    %esp,%ebp
 298:	53                   	push   %ebx
 299:	83 ec 04             	sub    $0x4,%esp
 29c:	e8 0f 03 00 00       	call   5b0 <__x86.get_pc_thunk.bx>
 2a1:	81 c3 3f 03 00 00    	add    $0x33f,%ebx
 2a7:	83 7d 08 01          	cmpl   $0x1,0x8(%ebp)
 2ab:	75 1a                	jne    2c7 <window_event_handler+0x32>
 2ad:	83 ec 0c             	sub    $0xc,%esp
 2b0:	8d 83 ba ff ff ff    	lea    -0x46(%ebx),%eax
 2b6:	50                   	push   %eax
 2b7:	e8 30 03 00 00       	call   5ec <print>
 2bc:	83 c4 10             	add    $0x10,%esp
 2bf:	c6 83 00 fd ff ff 00 	movb   $0x0,-0x300(%ebx)
 2c6:	90                   	nop
 2c7:	90                   	nop
 2c8:	8b 5d fc             	mov    -0x4(%ebp),%ebx
 2cb:	c9                   	leave  
 2cc:	c3                   	ret    
 2cd:	66 90                	xchg   %ax,%ax
 2cf:	66 90                	xchg   %ax,%ax
 2d1:	66 90                	xchg   %ax,%ax
 2d3:	66 90                	xchg   %ax,%ax
 2d5:	66 90                	xchg   %ax,%ax
 2d7:	66 90                	xchg   %ax,%ax
 2d9:	66 90                	xchg   %ax,%ax
 2db:	66 90                	xchg   %ax,%ax
 2dd:	66 90                	xchg   %ax,%ax
 2df:	90                   	nop

000002e0 <run>:
 2e0:	01 00 00 00                                         ....

000002e4 <color_sequence>:
 2e4:	ff b6 00 00 00 b6 ff 00 00 ff b6 00 ff 00 b6 00     ................
 2f4:	b6 00 ff 00 b6 ff 00 00 00 00 00 00                 ............

00000300 <block_map>:
	...
 334:	0f 00 00 00 0f 00 00 00 0f 00 00 00 0f 00 00 00     ................
 344:	0f 00 00 00 0f 00 00 00 0f 00 00 00 0f 00 00 00     ................
 354:	0f 00 00 00 0f 00 00 00 00 00 00 00 00 00 00 00     ................
 364:	0f 00 00 00 0f 00 00 00 ff ff ff 00 0f 00 00 00     ................
 374:	0f 00 00 00 0f 00 00 00 0f 00 00 00 0f 00 00 00     ................
 384:	0f 00 00 00 0f 00 00 00 00 00 00 00 00 00 00 00     ................
 394:	0f 00 00 00 ff ff ff 00 ff ff ff 00 0f 00 00 00     ................
 3a4:	0f 00 00 00 0f 00 00 00 0f 00 00 00 0f 00 00 00     ................
 3b4:	0f 00 00 00 0f 00 00 00 00 00 00 00 00 00 00 00     ................
 3c4:	0f 00 00 00 0f 00 00 00 0f 00 00 00 0f 00 00 00     ................
 3d4:	0f 00 00 00 0f 00 00 00 0f 00 00 00 0f 00 00 00     ................
 3e4:	0f 00 00 00 0f 00 00 00 00 00 00 00 00 00 00 00     ................
 3f4:	0f 00 00 00 0f 00 00 00 0f 00 00 00 0f 00 00 00     ................
 404:	0f 00 00 00 0f 00 00 00 0f 00 00 00 0f 00 00 00     ................
 414:	0f 00 00 00 0f 00 00 00 00 00 00 00 00 00 00 00     ................
 424:	0f 00 00 00 0f 00 00 00 0f 00 00 00 0f 00 00 00     ................
 434:	0f 00 00 00 0f 00 00 00 0f 00 00 00 0f 00 00 00     ................
 444:	0f 00 00 00 0f 00 00 00 00 00 00 00 00 00 00 00     ................
 454:	0f 00 00 00 0f 00 00 00 0f 00 00 00 0f 00 00 00     ................
 464:	0f 00 00 00 0f 00 00 00 0f 00 00 00 0f 00 00 00     ................
 474:	0f 00 00 00 0f 00 00 00 00 00 00 00 00 00 00 00     ................
 484:	0f 00 00 00 0f 00 00 00 0f 00 00 00 0f 00 00 00     ................
 494:	0f 00 00 00 0f 00 00 00 0f 00 00 00 0f 00 00 00     ................
 4a4:	0f 00 00 00 0f 00 00 00 00 00 00 00 00 00 00 00     ................
 4b4:	0f 00 00 00 0f 00 00 00 0f 00 00 00 0f 00 00 00     ................
 4c4:	0f 00 00 00 0f 00 00 00 0f 00 00 00 0f 00 00 00     ................
 4d4:	0f 00 00 00 0f 00 00 00 00 00 00 00 00 00 00 00     ................
 4e4:	0f 00 00 00 0f 00 00 00 0f 00 00 00 0f 00 00 00     ................
 4f4:	0f 00 00 00 0f 00 00 00 0f 00 00 00 0f 00 00 00     ................
 504:	0f 00 00 00 0f 00 00 00 00 00 00 00 00 00 00 00     ................
	...

00000540 <title_map>:
 540:	bc 0f 00 00 20 02 00 00 3c 02 00 00 20 02 00 00     .... ...<... ...
 550:	3c 02 00 00 54 45 54 52 49 53 20 54 49 4d 45 0a     <...TETRIS TIME.
 560:	00 54 65 74 72 69 73 20 57 69 6e 64 6f 77 20 4f     .Tetris Window O
 570:	70 65 6e 0a 00 45 73 63 61 70 65 20 50 72 65 73     pen..Escape Pres
 580:	73 65 64 0a 00 54 65 74 72 69 73 20 57 69 6e 64     sed..Tetris Wind
 590:	6f 77 20 43 6c 6f 73 65 0a 00 57 69 6e 64 6f 77     ow Close..Window
 5a0:	20 43 6c 6f 73 65 0a 00                              Close..

000005a8 <__x86.get_pc_thunk.ax>:
 5a8:	8b 04 24             	mov    (%esp),%eax
 5ab:	c3                   	ret    

000005ac <__x86.get_pc_thunk.dx>:
 5ac:	8b 14 24             	mov    (%esp),%edx
 5af:	c3                   	ret    

000005b0 <__x86.get_pc_thunk.bx>:
 5b0:	8b 1c 24             	mov    (%esp),%ebx
 5b3:	c3                   	ret    
 5b4:	47                   	inc    %edi
 5b5:	43                   	inc    %ebx
 5b6:	43                   	inc    %ebx
 5b7:	3a 20                	cmp    (%eax),%ah
 5b9:	28 55 62             	sub    %dl,0x62(%ebp)
 5bc:	75 6e                	jne    62c <fopen+0x5>
 5be:	74 75                	je     635 <fopen+0xe>
 5c0:	20 39                	and    %bh,(%ecx)
 5c2:	2e 34 2e             	cs xor $0x2e,%al
 5c5:	30 2d 31 75 62 75    	xor    %ch,0x75627531
 5cb:	6e                   	outsb  %ds:(%esi),(%dx)
 5cc:	74 75                	je     643 <fclose+0x1>
 5ce:	31 7e 32             	xor    %edi,0x32(%esi)
 5d1:	30 2e                	xor    %ch,(%esi)
 5d3:	30 34 2e             	xor    %dh,(%esi,%ebp,1)
 5d6:	31 29                	xor    %ebp,(%ecx)
 5d8:	20 39                	and    %bh,(%ecx)
 5da:	2e 34 2e             	cs xor $0x2e,%al
 5dd:	30 00                	xor    %al,(%eax)
 5df:	90                   	nop

000005e0 <_GLOBAL_OFFSET_TABLE_>:
	...

000005ec <print>:
 5ec:	55                   	push   %ebp
 5ed:	89 e5                	mov    %esp,%ebp
 5ef:	53                   	push   %ebx
 5f0:	e8 b3 ff ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 5f5:	05 eb ff ff ff       	add    $0xffffffeb,%eax
 5fa:	b8 01 00 00 00       	mov    $0x1,%eax
 5ff:	8b 5d 08             	mov    0x8(%ebp),%ebx
 602:	cd 80                	int    $0x80
 604:	90                   	nop
 605:	5b                   	pop    %ebx
 606:	5d                   	pop    %ebp
 607:	c3                   	ret    

00000608 <printval>:
 608:	55                   	push   %ebp
 609:	89 e5                	mov    %esp,%ebp
 60b:	53                   	push   %ebx
 60c:	e8 97 ff ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 611:	05 cf ff ff ff       	add    $0xffffffcf,%eax
 616:	b8 03 00 00 00       	mov    $0x3,%eax
 61b:	8b 5d 08             	mov    0x8(%ebp),%ebx
 61e:	8b 55 0c             	mov    0xc(%ebp),%edx
 621:	cd 80                	int    $0x80
 623:	90                   	nop
 624:	5b                   	pop    %ebx
 625:	5d                   	pop    %ebp
 626:	c3                   	ret    

00000627 <fopen>:
 627:	55                   	push   %ebp
 628:	89 e5                	mov    %esp,%ebp
 62a:	53                   	push   %ebx
 62b:	e8 78 ff ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 630:	05 b0 ff ff ff       	add    $0xffffffb0,%eax
 635:	b8 11 00 00 00       	mov    $0x11,%eax
 63a:	8b 5d 08             	mov    0x8(%ebp),%ebx
 63d:	cd 80                	int    $0x80
 63f:	5b                   	pop    %ebx
 640:	5d                   	pop    %ebp
 641:	c3                   	ret    

00000642 <fclose>:
 642:	55                   	push   %ebp
 643:	89 e5                	mov    %esp,%ebp
 645:	53                   	push   %ebx
 646:	e8 5d ff ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 64b:	05 95 ff ff ff       	add    $0xffffff95,%eax
 650:	b8 12 00 00 00       	mov    $0x12,%eax
 655:	8b 5d 08             	mov    0x8(%ebp),%ebx
 658:	cd 80                	int    $0x80
 65a:	5b                   	pop    %ebx
 65b:	5d                   	pop    %ebp
 65c:	c3                   	ret    

0000065d <fread>:
 65d:	55                   	push   %ebp
 65e:	89 e5                	mov    %esp,%ebp
 660:	53                   	push   %ebx
 661:	e8 42 ff ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 666:	05 7a ff ff ff       	add    $0xffffff7a,%eax
 66b:	b8 13 00 00 00       	mov    $0x13,%eax
 670:	8b 55 08             	mov    0x8(%ebp),%edx
 673:	89 d3                	mov    %edx,%ebx
 675:	8b 55 0c             	mov    0xc(%ebp),%edx
 678:	89 d1                	mov    %edx,%ecx
 67a:	cd 80                	int    $0x80
 67c:	5b                   	pop    %ebx
 67d:	5d                   	pop    %ebp
 67e:	c3                   	ret    

0000067f <image_load_buffer>:
 67f:	55                   	push   %ebp
 680:	89 e5                	mov    %esp,%ebp
 682:	53                   	push   %ebx
 683:	e8 20 ff ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 688:	05 58 ff ff ff       	add    $0xffffff58,%eax
 68d:	8b 45 08             	mov    0x8(%ebp),%eax
 690:	89 c3                	mov    %eax,%ebx
 692:	8b 45 0c             	mov    0xc(%ebp),%eax
 695:	89 c1                	mov    %eax,%ecx
 697:	8b 55 10             	mov    0x10(%ebp),%edx
 69a:	b8 0b 00 00 00       	mov    $0xb,%eax
 69f:	cd 80                	int    $0x80
 6a1:	90                   	nop
 6a2:	5b                   	pop    %ebx
 6a3:	5d                   	pop    %ebp
 6a4:	c3                   	ret    

000006a5 <draw_image>:
 6a5:	55                   	push   %ebp
 6a6:	89 e5                	mov    %esp,%ebp
 6a8:	53                   	push   %ebx
 6a9:	e8 fa fe ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 6ae:	05 32 ff ff ff       	add    $0xffffff32,%eax
 6b3:	b8 0c 00 00 00       	mov    $0xc,%eax
 6b8:	8b 5d 08             	mov    0x8(%ebp),%ebx
 6bb:	8b 4d 0c             	mov    0xc(%ebp),%ecx
 6be:	cd 80                	int    $0x80
 6c0:	90                   	nop
 6c1:	5b                   	pop    %ebx
 6c2:	5d                   	pop    %ebp
 6c3:	c3                   	ret    

000006c4 <malloc>:
 6c4:	55                   	push   %ebp
 6c5:	89 e5                	mov    %esp,%ebp
 6c7:	53                   	push   %ebx
 6c8:	e8 db fe ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 6cd:	05 13 ff ff ff       	add    $0xffffff13,%eax
 6d2:	b8 06 00 00 00       	mov    $0x6,%eax
 6d7:	8b 5d 08             	mov    0x8(%ebp),%ebx
 6da:	cd 80                	int    $0x80
 6dc:	5b                   	pop    %ebx
 6dd:	5d                   	pop    %ebp
 6de:	c3                   	ret    

000006df <free>:
 6df:	55                   	push   %ebp
 6e0:	89 e5                	mov    %esp,%ebp
 6e2:	53                   	push   %ebx
 6e3:	e8 c0 fe ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 6e8:	05 f8 fe ff ff       	add    $0xfffffef8,%eax
 6ed:	b8 07 00 00 00       	mov    $0x7,%eax
 6f2:	8b 45 08             	mov    0x8(%ebp),%eax
 6f5:	89 c3                	mov    %eax,%ebx
 6f7:	cd 80                	int    $0x80
 6f9:	90                   	nop
 6fa:	5b                   	pop    %ebx
 6fb:	5d                   	pop    %ebp
 6fc:	c3                   	ret    

000006fd <toggle_console>:
 6fd:	55                   	push   %ebp
 6fe:	89 e5                	mov    %esp,%ebp
 700:	e8 a3 fe ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 705:	05 db fe ff ff       	add    $0xfffffedb,%eax
 70a:	b8 0f 00 00 00       	mov    $0xf,%eax
 70f:	cd 80                	int    $0x80
 711:	90                   	nop
 712:	5d                   	pop    %ebp
 713:	c3                   	ret    

00000714 <get_ascii_key_index>:
 714:	55                   	push   %ebp
 715:	89 e5                	mov    %esp,%ebp
 717:	53                   	push   %ebx
 718:	e8 8b fe ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 71d:	05 c3 fe ff ff       	add    $0xfffffec3,%eax
 722:	b8 0a 00 00 00       	mov    $0xa,%eax
 727:	cd 80                	int    $0x80
 729:	89 d8                	mov    %ebx,%eax
 72b:	5b                   	pop    %ebx
 72c:	5d                   	pop    %ebp
 72d:	c3                   	ret    

0000072e <get_ascii_key>:
 72e:	55                   	push   %ebp
 72f:	89 e5                	mov    %esp,%ebp
 731:	53                   	push   %ebx
 732:	e8 71 fe ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 737:	05 a9 fe ff ff       	add    $0xfffffea9,%eax
 73c:	b8 09 00 00 00       	mov    $0x9,%eax
 741:	cd 80                	int    $0x80
 743:	89 d8                	mov    %ebx,%eax
 745:	5b                   	pop    %ebx
 746:	5d                   	pop    %ebp
 747:	c3                   	ret    

00000748 <clear_screen>:
 748:	55                   	push   %ebp
 749:	89 e5                	mov    %esp,%ebp
 74b:	53                   	push   %ebx
 74c:	e8 57 fe ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 751:	05 8f fe ff ff       	add    $0xfffffe8f,%eax
 756:	8b 5d 08             	mov    0x8(%ebp),%ebx
 759:	b8 10 00 00 00       	mov    $0x10,%eax
 75e:	cd 80                	int    $0x80
 760:	90                   	nop
 761:	5b                   	pop    %ebx
 762:	5d                   	pop    %ebp
 763:	c3                   	ret    

00000764 <get_display>:
 764:	55                   	push   %ebp
 765:	89 e5                	mov    %esp,%ebp
 767:	53                   	push   %ebx
 768:	83 ec 10             	sub    $0x10,%esp
 76b:	e8 38 fe ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 770:	05 70 fe ff ff       	add    $0xfffffe70,%eax
 775:	b8 08 00 00 00       	mov    $0x8,%eax
 77a:	cd 80                	int    $0x80
 77c:	89 d8                	mov    %ebx,%eax
 77e:	89 45 f4             	mov    %eax,-0xc(%ebp)
 781:	89 c8                	mov    %ecx,%eax
 783:	89 45 f8             	mov    %eax,-0x8(%ebp)
 786:	8b 4d 08             	mov    0x8(%ebp),%ecx
 789:	8b 45 f4             	mov    -0xc(%ebp),%eax
 78c:	8b 55 f8             	mov    -0x8(%ebp),%edx
 78f:	89 01                	mov    %eax,(%ecx)
 791:	89 51 04             	mov    %edx,0x4(%ecx)
 794:	8b 45 08             	mov    0x8(%ebp),%eax
 797:	83 c4 10             	add    $0x10,%esp
 79a:	5b                   	pop    %ebx
 79b:	5d                   	pop    %ebp
 79c:	c2 04 00             	ret    $0x4

0000079f <get_timer_ticks>:
 79f:	55                   	push   %ebp
 7a0:	89 e5                	mov    %esp,%ebp
 7a2:	e8 01 fe ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 7a7:	05 39 fe ff ff       	add    $0xfffffe39,%eax
 7ac:	b8 14 00 00 00       	mov    $0x14,%eax
 7b1:	cd 80                	int    $0x80
 7b3:	5d                   	pop    %ebp
 7b4:	c3                   	ret    

000007b5 <set_backbuffer>:
 7b5:	55                   	push   %ebp
 7b6:	89 e5                	mov    %esp,%ebp
 7b8:	53                   	push   %ebx
 7b9:	83 ec 04             	sub    $0x4,%esp
 7bc:	e8 e7 fd ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 7c1:	05 1f fe ff ff       	add    $0xfffffe1f,%eax
 7c6:	8b 45 08             	mov    0x8(%ebp),%eax
 7c9:	88 45 f8             	mov    %al,-0x8(%ebp)
 7cc:	0f b6 5d f8          	movzbl -0x8(%ebp),%ebx
 7d0:	b8 15 00 00 00       	mov    $0x15,%eax
 7d5:	cd 80                	int    $0x80
 7d7:	90                   	nop
 7d8:	83 c4 04             	add    $0x4,%esp
 7db:	5b                   	pop    %ebx
 7dc:	5d                   	pop    %ebp
 7dd:	c3                   	ret    

000007de <swap_buffers>:
 7de:	55                   	push   %ebp
 7df:	89 e5                	mov    %esp,%ebp
 7e1:	e8 c2 fd ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 7e6:	05 fa fd ff ff       	add    $0xfffffdfa,%eax
 7eb:	b8 16 00 00 00       	mov    $0x16,%eax
 7f0:	cd 80                	int    $0x80
 7f2:	90                   	nop
 7f3:	5d                   	pop    %ebp
 7f4:	c3                   	ret    

000007f5 <get_key>:
 7f5:	55                   	push   %ebp
 7f6:	89 e5                	mov    %esp,%ebp
 7f8:	53                   	push   %ebx
 7f9:	83 ec 10             	sub    $0x10,%esp
 7fc:	e8 af fd ff ff       	call   5b0 <__x86.get_pc_thunk.bx>
 801:	81 c3 df fd ff ff    	add    $0xfffffddf,%ebx
 807:	e8 08 ff ff ff       	call   714 <get_ascii_key_index>
 80c:	89 45 f8             	mov    %eax,-0x8(%ebp)
 80f:	c7 c0 24 b0 0e 00    	mov    $0xeb024,%eax
 815:	8b 00                	mov    (%eax),%eax
 817:	39 45 f8             	cmp    %eax,-0x8(%ebp)
 81a:	74 12                	je     82e <get_key+0x39>
 81c:	c7 c0 24 b0 0e 00    	mov    $0xeb024,%eax
 822:	8b 55 f8             	mov    -0x8(%ebp),%edx
 825:	89 10                	mov    %edx,(%eax)
 827:	e8 02 ff ff ff       	call   72e <get_ascii_key>
 82c:	eb 05                	jmp    833 <get_key+0x3e>
 82e:	b8 00 00 00 00       	mov    $0x0,%eax
 833:	83 c4 10             	add    $0x10,%esp
 836:	5b                   	pop    %ebx
 837:	5d                   	pop    %ebp
 838:	c3                   	ret    

00000839 <get_keycode_index>:
 839:	55                   	push   %ebp
 83a:	89 e5                	mov    %esp,%ebp
 83c:	53                   	push   %ebx
 83d:	e8 66 fd ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 842:	05 9e fd ff ff       	add    $0xfffffd9e,%eax
 847:	b8 19 00 00 00       	mov    $0x19,%eax
 84c:	cd 80                	int    $0x80
 84e:	89 d8                	mov    %ebx,%eax
 850:	5b                   	pop    %ebx
 851:	5d                   	pop    %ebp
 852:	c3                   	ret    

00000853 <get_keycode>:
 853:	55                   	push   %ebp
 854:	89 e5                	mov    %esp,%ebp
 856:	53                   	push   %ebx
 857:	e8 4c fd ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 85c:	05 84 fd ff ff       	add    $0xfffffd84,%eax
 861:	b8 18 00 00 00       	mov    $0x18,%eax
 866:	cd 80                	int    $0x80
 868:	89 d8                	mov    %ebx,%eax
 86a:	5b                   	pop    %ebx
 86b:	5d                   	pop    %ebp
 86c:	c3                   	ret    

0000086d <get_keycode_next>:
 86d:	55                   	push   %ebp
 86e:	89 e5                	mov    %esp,%ebp
 870:	53                   	push   %ebx
 871:	83 ec 10             	sub    $0x10,%esp
 874:	e8 37 fd ff ff       	call   5b0 <__x86.get_pc_thunk.bx>
 879:	81 c3 67 fd ff ff    	add    $0xfffffd67,%ebx
 87f:	e8 b5 ff ff ff       	call   839 <get_keycode_index>
 884:	89 45 f8             	mov    %eax,-0x8(%ebp)
 887:	c7 c0 20 b0 0e 00    	mov    $0xeb020,%eax
 88d:	8b 00                	mov    (%eax),%eax
 88f:	39 45 f8             	cmp    %eax,-0x8(%ebp)
 892:	74 12                	je     8a6 <get_keycode_next+0x39>
 894:	c7 c0 20 b0 0e 00    	mov    $0xeb020,%eax
 89a:	8b 55 f8             	mov    -0x8(%ebp),%edx
 89d:	89 10                	mov    %edx,(%eax)
 89f:	e8 af ff ff ff       	call   853 <get_keycode>
 8a4:	eb 05                	jmp    8ab <get_keycode_next+0x3e>
 8a6:	b8 00 00 00 00       	mov    $0x0,%eax
 8ab:	83 c4 10             	add    $0x10,%esp
 8ae:	5b                   	pop    %ebx
 8af:	5d                   	pop    %ebp
 8b0:	c3                   	ret    

000008b1 <printChar>:
 8b1:	55                   	push   %ebp
 8b2:	89 e5                	mov    %esp,%ebp
 8b4:	53                   	push   %ebx
 8b5:	83 ec 04             	sub    $0x4,%esp
 8b8:	e8 eb fc ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 8bd:	05 23 fd ff ff       	add    $0xfffffd23,%eax
 8c2:	8b 45 10             	mov    0x10(%ebp),%eax
 8c5:	88 45 f8             	mov    %al,-0x8(%ebp)
 8c8:	b8 04 00 00 00       	mov    $0x4,%eax
 8cd:	8b 5d 08             	mov    0x8(%ebp),%ebx
 8d0:	8b 4d 0c             	mov    0xc(%ebp),%ecx
 8d3:	0f b6 55 f8          	movzbl -0x8(%ebp),%edx
 8d7:	cd 80                	int    $0x80
 8d9:	90                   	nop
 8da:	83 c4 04             	add    $0x4,%esp
 8dd:	5b                   	pop    %ebx
 8de:	5d                   	pop    %ebp
 8df:	c3                   	ret    

000008e0 <setPrintColor>:
 8e0:	55                   	push   %ebp
 8e1:	89 e5                	mov    %esp,%ebp
 8e3:	53                   	push   %ebx
 8e4:	e8 bf fc ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 8e9:	05 f7 fc ff ff       	add    $0xfffffcf7,%eax
 8ee:	b8 1a 00 00 00       	mov    $0x1a,%eax
 8f3:	8b 5d 08             	mov    0x8(%ebp),%ebx
 8f6:	cd 80                	int    $0x80
 8f8:	90                   	nop
 8f9:	5b                   	pop    %ebx
 8fa:	5d                   	pop    %ebp
 8fb:	c3                   	ret    

000008fc <getFramebuffer>:
 8fc:	55                   	push   %ebp
 8fd:	89 e5                	mov    %esp,%ebp
 8ff:	e8 a4 fc ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 904:	05 dc fc ff ff       	add    $0xfffffcdc,%eax
 909:	b8 17 00 00 00       	mov    $0x17,%eax
 90e:	cd 80                	int    $0x80
 910:	5d                   	pop    %ebp
 911:	c3                   	ret    

00000912 <toggle_mouse>:
 912:	55                   	push   %ebp
 913:	89 e5                	mov    %esp,%ebp
 915:	e8 8e fc ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 91a:	05 c6 fc ff ff       	add    $0xfffffcc6,%eax
 91f:	b8 1b 00 00 00       	mov    $0x1b,%eax
 924:	cd 80                	int    $0x80
 926:	90                   	nop
 927:	5d                   	pop    %ebp
 928:	c3                   	ret    

00000929 <mouse_draw>:
 929:	55                   	push   %ebp
 92a:	89 e5                	mov    %esp,%ebp
 92c:	e8 77 fc ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 931:	05 af fc ff ff       	add    $0xfffffcaf,%eax
 936:	b8 1c 00 00 00       	mov    $0x1c,%eax
 93b:	cd 80                	int    $0x80
 93d:	90                   	nop
 93e:	5d                   	pop    %ebp
 93f:	c3                   	ret    

00000940 <get_mouse_data>:
 940:	55                   	push   %ebp
 941:	89 e5                	mov    %esp,%ebp
 943:	53                   	push   %ebx
 944:	83 ec 10             	sub    $0x10,%esp
 947:	e8 5c fc ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 94c:	05 94 fc ff ff       	add    $0xfffffc94,%eax
 951:	b8 1d 00 00 00       	mov    $0x1d,%eax
 956:	cd 80                	int    $0x80
 958:	89 d8                	mov    %ebx,%eax
 95a:	89 45 f0             	mov    %eax,-0x10(%ebp)
 95d:	89 c8                	mov    %ecx,%eax
 95f:	89 45 f4             	mov    %eax,-0xc(%ebp)
 962:	89 d0                	mov    %edx,%eax
 964:	89 45 f8             	mov    %eax,-0x8(%ebp)
 967:	8b 45 08             	mov    0x8(%ebp),%eax
 96a:	8b 55 f0             	mov    -0x10(%ebp),%edx
 96d:	89 10                	mov    %edx,(%eax)
 96f:	8b 55 f4             	mov    -0xc(%ebp),%edx
 972:	89 50 04             	mov    %edx,0x4(%eax)
 975:	8b 55 f8             	mov    -0x8(%ebp),%edx
 978:	89 50 08             	mov    %edx,0x8(%eax)
 97b:	8b 45 08             	mov    0x8(%ebp),%eax
 97e:	83 c4 10             	add    $0x10,%esp
 981:	5b                   	pop    %ebx
 982:	5d                   	pop    %ebp
 983:	c2 04 00             	ret    $0x4

00000986 <toggle_window>:
 986:	55                   	push   %ebp
 987:	89 e5                	mov    %esp,%ebp
 989:	e8 1a fc ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 98e:	05 52 fc ff ff       	add    $0xfffffc52,%eax
 993:	b8 1e 00 00 00       	mov    $0x1e,%eax
 998:	cd 80                	int    $0x80
 99a:	90                   	nop
 99b:	5d                   	pop    %ebp
 99c:	c3                   	ret    

0000099d <open_window>:
 99d:	55                   	push   %ebp
 99e:	89 e5                	mov    %esp,%ebp
 9a0:	53                   	push   %ebx
 9a1:	e8 02 fc ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 9a6:	05 3a fc ff ff       	add    $0xfffffc3a,%eax
 9ab:	8b 55 10             	mov    0x10(%ebp),%edx
 9ae:	8b 4d 0c             	mov    0xc(%ebp),%ecx
 9b1:	8b 5d 08             	mov    0x8(%ebp),%ebx
 9b4:	b8 1f 00 00 00       	mov    $0x1f,%eax
 9b9:	cd 80                	int    $0x80
 9bb:	89 d8                	mov    %ebx,%eax
 9bd:	5b                   	pop    %ebx
 9be:	5d                   	pop    %ebp
 9bf:	c3                   	ret    

000009c0 <close_window>:
 9c0:	55                   	push   %ebp
 9c1:	89 e5                	mov    %esp,%ebp
 9c3:	53                   	push   %ebx
 9c4:	e8 df fb ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 9c9:	05 17 fc ff ff       	add    $0xfffffc17,%eax
 9ce:	8b 5d 08             	mov    0x8(%ebp),%ebx
 9d1:	b8 20 00 00 00       	mov    $0x20,%eax
 9d6:	cd 80                	int    $0x80
 9d8:	90                   	nop
 9d9:	5b                   	pop    %ebx
 9da:	5d                   	pop    %ebp
 9db:	c3                   	ret    

000009dc <add_window_event_handler>:
 9dc:	55                   	push   %ebp
 9dd:	89 e5                	mov    %esp,%ebp
 9df:	53                   	push   %ebx
 9e0:	e8 c3 fb ff ff       	call   5a8 <__x86.get_pc_thunk.ax>
 9e5:	05 fb fb ff ff       	add    $0xfffffbfb,%eax
 9ea:	8b 4d 0c             	mov    0xc(%ebp),%ecx
 9ed:	8b 5d 08             	mov    0x8(%ebp),%ebx
 9f0:	b8 21 00 00 00       	mov    $0x21,%eax
 9f5:	cd 80                	int    $0x80
 9f7:	90                   	nop
 9f8:	5b                   	pop    %ebx
 9f9:	5d                   	pop    %ebp
 9fa:	c3                   	ret    

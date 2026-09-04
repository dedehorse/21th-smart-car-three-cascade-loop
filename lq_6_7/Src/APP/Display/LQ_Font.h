/*******************************************************************************
 *  @file                 本文件是LQ_TC387_Software_Library 软件开源库文件的一部分
 *  @author               chiusir
 *  @email                chiusir@163.com
 *  @version              V2.0.0
 *  @update               2026年3月24日
 *  @copyright            版权所有 (C) 2025-2026 北京龙邱科技有限公司
 *  @website              http://www.lqist.cn
 *  @taobao               http://longqiu.taobao.com
 *
 *  @brief                龙邱科技 LQ_TC387核心板驱动库声明
 *
 * 本软件遵循GPL-3.0开源协议发布，旨在为TC387芯片嵌入式系统设计者提供快速上手、开发基于TC387应用程序的参考
 * 商业用途（包括单位使用）需提前联系 http://www.lqist.cn 获得授权
 *
 * 开发环境配置:
 *   - 开发平台 : AURIX-Studio 版本-1.10.2  (简称ADS 1.10.x)
 *   - 文件编码 : UTF-8  (AURIX-Studio-1.10.x 默认编码)
 *   - 目标芯片 : TC387QP (TriCore™ @ 300 MHz X4 10 Mbyte flash, 1568 KB of RAM)
 *   - 外置晶振 : 20MHz
 *   - 系统PLL : 300MHz + 300MHz + 300MHz + 300MHz
 * 
 * GPL-3.0 许可证声明摘要:
 * 1. 允许自由使用、修改、分发本软件
 * 2. 分发修改后的版本时，必须以相同许可证发布
 * 3. 必须保留原始版权声明和许可证信息
 * 4. 不提供任何担保，使用风险自负
 * 5. 完整协议文本请参见项目根目录 LICENSE 文件
 *******************************************************************************/

#ifndef __LQ_Font_H__
#define __LQ_Font_H__
/** 字库
字库码的引索
--------------------------------------------------------------
(0)  !(1)  "(2)  #(3)  $(4)  %(5)  &(6)  '(7)
((8)  )(9)  *(10) +(11) ,(12) -(13) .(14) /(15)
0(16) 1(17) 2(18) 3(19) 4(20) 5(21) 6(22) 7(23)
8(24) 9(25) :(26) ;(27) <(28) =(29) >(30) ?(31)
@(32) A(33) B(34) C(35) D(36) E(37) F(38) G(39)
H(40) I(41) J(42) K(43) L(44) M(45) N(46) O(47)
P(48) Q(49) R(50) S(51) T(52) U(53) V(54) W(55)
X(56) Y(57) Z(58) [(59) \(60) ](61) ^(62) _(63)
`(64) a(65) b(66) c(67) d(68) e(69) f(70) g(71)
h(72) i(73) j(74) k(75) l(76) m(77) n(78) o(79)
p(80) q(81) r(82) s(83) t(84) u(85) v(86) w(87)
x(88) y(89) z(90) {(91) |(92) }(93) ~(94)
--------------------------------------------------------------
字体:   8*6字体
取码规则:   低位在前, 列行扫描, 阴码(1-亮, 0-灭)
查表方法: 要显示的ASCII码-32就可以得到字库码的指针 */
extern const unsigned char Font_code8[][6];

/** 字库
字库码的引索
--------------------------------------------------------------
(0)  !(1)  "(2)  #(3)  $(4)  %(5)  &(6)  '(7)
((8)  )(9)  *(10) +(11) ,(12) -(13) .(14) /(15)
0(16) 1(17) 2(18) 3(19) 4(20) 5(21) 6(22) 7(23)
8(24) 9(25) :(26) ;(27) <(28) =(29) >(30) ?(31)
@(32) A(33) B(34) C(35) D(36) E(37) F(38) G(39)
H(40) I(41) J(42) K(43) L(44) M(45) N(46) O(47)
P(48) Q(49) R(50) S(51) T(52) U(53) V(54) W(55)
X(56) Y(57) Z(58) [(59) \(60) ](61) ^(62) _(63)
`(64) a(65) b(66) c(67) d(68) e(69) f(70) g(71)
h(72) i(73) j(74) k(75) l(76) m(77) n(78) o(79)
p(80) q(81) r(82) s(83) t(84) u(85) v(86) w(87)
x(88) y(89) z(90) {(91) |(92) }(93) ~(94)
--------------------------------------------------------------
字体:   GulimChe12 8*16字体
取码规则:   低位在前, 列行扫描, 阴码(1-亮, 0-灭)
查表方法: 要显示的ASCII码-32就可以得到字库码的指针 */
extern const unsigned char Font_code16[][16];



/** 16*16 简单的汉字字库*/
extern const unsigned char hanzi16x16[];
extern const unsigned char gImage_1[3200];
extern const unsigned char gImage_title2[5152];
extern const unsigned char gImage_Battery_S[400];
extern const unsigned char gImage_lo[17280];
extern const unsigned char Font_code16[][16];
extern const unsigned char Font_code8[][6];
extern const unsigned char hanzi_Idx[];
extern const unsigned char hanzi16x16[];
extern const unsigned char ascii_1206[][12];
extern const unsigned char ascii_1608[][16];
extern const unsigned char ascii_2412[][48];
extern const unsigned char ascii_3216[][64];
extern const unsigned char tfont_Idx[];
extern const unsigned char tfont_12x12[];
extern const unsigned char tfont_16x16[];
extern const unsigned char tfont_24x24[];
extern const unsigned char tfont_32x32[];
#endif

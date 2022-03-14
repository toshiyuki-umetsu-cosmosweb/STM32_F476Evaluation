/*
 * debug.h
 *
 *  Created on: Mar 14, 2022
 *      Author: toshiyuki_umetsu
 */

#ifndef SRC_DEBUG_H_
#define SRC_DEBUG_H_

#ifndef _DEBUG
/* DebugFlag
 *  もしデバッグモードにしないならば、_DEBUGはコメントアウトする。
 *  コンパイラに渡すオプションで-D_DEBUGが渡されている場合にはそちらが優先される。
 */
#define _DEBUG
#endif

#ifdef _DEBUG
#include <stddef.h>
#include <stdio.h>

#define dprintf printf

#ifdef __cplusplus
extern "C" {
#endif



#ifdef __cplusplus
}
#endif
#else
#define dprintf()
#endif

#endif /* SRC_DEBUG_H_ */

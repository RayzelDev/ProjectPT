#ifndef	_NET_TYPE_HEADER_
#define _NET_TYPE_HEADER_

//#define	_W_SERVER
//#define	_W_DATA_SERVER

//New items authentication code applies
#define _NEW_ITEM_FORMCODE_

//The new complex cryptographic authentication code applies to items (Chinese, English application server)
#define _NEW_ITEM_FORMCODE2
//More new hybrid encryption authentication code applies to the item (Thai / Japanese)
#define _NEW_ITEM_FORMCODE3

//#define _SERVER_MODE_OLD

#define _WINMODE_DEBUG

#ifdef _WINMODE_DEBUG
//#define _DEBUG_SOCKET
#endif

#ifdef _W_SERVER
//#define	_npGAME_GUARD_AUTH //XTrap 컴파일시 주석처리
#endif

#endif

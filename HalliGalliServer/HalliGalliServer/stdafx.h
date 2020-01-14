
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.


#include <afxdisp.h>        // MFC 자동화 클래스입니다.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

/* STL 관련 인클루드 */
#include <vector>
#include <list>
#include <map>
#include <algorithm>

using namespace std;

/* 소켓 관련 인클루드 및 Define */
#include <afxsock.h>

#include "SocSvr.h"

#define UM_ACCEPT	WM_USER+1
#define UM_RECEIVE	WM_USER+2

// 이벤트 처리 
#define SOC_INITGAME		0	// 전송 데이터: 나눠줄 카드 데이터(CARD 구조체 데이터)
#define SOC_GAMESTART		1	// 전송 데이터: X
#define SOC_GAMEEND			2	// 전송 데이터: 상대의 승패 여부
#define SOC_TEXT			3	// 전송 데이터: 보내는 문자열(m_strSend)
#define SOC_THROWNCARD		4	// 전송 데이터: 내는 카드
#define SOC_BELL			5	// 전송 데이터: X
#define SOC_TAKECARD		6	// 전송 데이터: X
#define SOC_NOTAKECARD		7	// 전송 데이터: 패널티 카드 한장 전송
#define SOC_NOTURN			8	// 전송 데이터: 내 카드 개수

#define DEFAULT_PORT	5000
#define DEFAULT_IP		"127.0.0.1"

/* 사운드*/
#include "fmod.h"
#pragma comment(lib, "fmodex_vc.lib")

class CSoundMgr;
extern CSoundMgr* g_pSoundMgr;

/* 기타 */
#include <afxpriv.h>

#include "Struct.h"
#include "Macro.h"

#include "ImageMgr.h" // 이미지 관리 클래스

#define CARD_CNT 28
#define CARD_HALF_CNT 14

#define TURN_CNT	50

#define GAME_WIN	0	// 상대가 이김	
#define GAME_LOSE	1	// 상대가 짐
#define GAME_DRAW	2	// 무승부

#define MID_STR		128
#define MAX_STR		256
	
extern float g_fDeltaTime;

#pragma warning(disable : 4985)
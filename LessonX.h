/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#ifndef _LESSON_X_H_
#define _LESSON_X_H_
//
#include <Windows.h>
#include<vector>
using namespace std;

/////////////////////////////////////////////////////////////////////////////////
//
// 游戏总管类。负责处理游戏主循环、游戏初始化、结束等工作
class	CGameMain
{
private:
	int				m_iGameState;				// 游戏状态，0：结束或者等待开始；1：初始化；2：游戏进行中
	CSprite*	m_pGameBegin;			//GameBegin精灵实例
	CSprite*	m_pGoldenHook;		//钩子精灵实例
	CAnimateSprite*   m_pGoldenMan;		//挖金人实例
	int m_iGetGoldState	;// 抓取金子状态：0 -- 等待按键开始抓金子；1 -- 钩子伸出去；2 -- 钩子往回伸，未抓到东西；3 -- 钩子往回伸，带着金子
	float m_fHookRotation;	// 钩子当前转动的朝向
	int m_iHookRotToLeft;	// 钩子当前是往左摆动还是往右摆动( 1 右-左， 0 左-右 )
	float m_fHookStartPosX;	// 钩子初始位置。在刚开始的时候初始化一次，用于钩子复位
	float m_fHookStartPosY;

	int	m_iGoldCount	;// 本局生成的金子数量，每抓取一个金子，数量递减一个，为0时本局结束
	CSprite*	m_pGoldenTemplate;	//默认绑定的金子精灵，用来创建其他金子
	vector<CSprite*> m_vGoldenBlock;	//金子精灵数组

	CSprite*	m_CurGetGold;			//当前抓到的金子对象
	float		m_fEmptyHookSpeed;		// 空的钩子的移动速度
public:
	CGameMain();            //构造函数
	~CGameMain();           //析构函数  

	// Get方法
	int				GetGameState()											{ return m_iGameState; }
	
	// Set方法
	void			SetGameState( const int iState )				{ m_iGameState	=	iState; }
	
	// 游戏主循环等
	void			GameMainLoop( float	fDeltaTime );
	void			GameInit();
	void			GameRun( float fDeltaTime );
	void			GameEnd();
	void 			OnMouseMove( const float fMouseX, const float fMouseY );
	void 			OnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY );
	void 			OnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY );
	void 			OnKeyDown( const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress );
	void 			OnKeyUp( const int iKey );
	void 			OnSpriteColSprite( const char *szSrcName, const char *szTarName );
	void 			OnSpriteColWorldLimit( const char *szName, const int iColSide );

	void			DrawHookLine();
	CSprite* CGameMain::GetCurGoldenSprite(const char* goldenname);

};

/////////////////////////////////////////////////////////////////////////////////
// 
extern CGameMain	g_GameMain;

#endif // _LESSON_X_H_
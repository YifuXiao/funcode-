/////////////////////////////////////////////////////////////////////////////////
//
//
//
//
/////////////////////////////////////////////////////////////////////////////////
#include <Stdio.h>
#include "CommonClass.h"
#include "LessonX.h"
#include<ctime>
////////////////////////////////////////////////////////////////////////////////
//
//
CGameMain		g_GameMain;	

//==============================================================================
//
// 大体的程序流程为：GameMainLoop函数为主循环函数，在引擎每帧刷新屏幕图像之后，都会被调用一次。

//==============================================================================
//
// 构造函数
CGameMain::CGameMain()
{
	m_iGameState			=	0;
	m_pGameBegin		=	new CSprite("GameBegin");
	m_iGetGoldState		=	0;
	m_fHookRotation		=	0.f;
	m_iHookRotToLeft		=	1;
	m_fHookStartPosX		=	0.f;
	m_fHookStartPosY		=	0.f;
	m_pGoldenHook		=	new CSprite("GoldHook");
	m_pGoldenMan		=	new CAnimateSprite("GoldMan");

	m_iGoldCount				=	0;
	m_pGoldenTemplate	=	new CSprite("GoldTemplate");

	m_fEmptyHookSpeed		=	15.f;
}
//==============================================================================
//
// 析构函数
CGameMain::~CGameMain()
{
}

//==============================================================================
//
// 游戏主循环，此函数将被不停的调用，引擎每刷新一次屏幕，此函数即被调用一次
// 用以处理游戏的开始、进行中、结束等各种状态. 
// 函数参数fDeltaTime : 上次调用本函数到此次调用本函数的时间间隔，单位：秒
void CGameMain::GameMainLoop( float	fDeltaTime )
{
	switch( GetGameState() )
	{
		// 初始化游戏，清空上一局相关数据
	case 1:
		{
			GameInit();
			SetGameState(2); // 初始化之后，将游戏状态设置为进行中
		}
		break;

		// 游戏进行中，处理各种游戏逻辑
	case 2:
		{
			// TODO 修改此处游戏循环条件，完成正确游戏逻辑
			if( true )
			{
				GameRun( fDeltaTime );
			}
			else // 游戏结束。调用游戏结算函数，并把游戏状态修改为结束状态
			{				
				SetGameState(0);
				GameEnd();
			}
		}
		break;

		// 游戏结束/等待按空格键开始
	case 0:
	default:
		break;
	};
	// 画钩子的缆绳线。不管游戏是什么状态，这根缆绳线都要画出来
	DrawHookLine();
}
//=============================================================================
//
// 每局开始前进行初始化，清空上一局相关数据
void CGameMain::GameInit()
{
	srand((unsigned int)time(NULL));//生成随机数
	// 重设钩子从0度、由右往左自动旋转
m_fHookRotation	=	0.f;
m_iHookRotToLeft	=	1;
// 初始化抓取状态
m_iGetGoldState	=	0;
// 钩子初始位置值初始化
m_fHookStartPosX	=	m_pGoldenHook->GetSpritePositionX();	
m_fHookStartPosY	=	m_pGoldenHook->GetSpritePositionY();	
//钩子的世界边界初始化
m_pGoldenHook->SetSpriteWorldLimit(WORLD_LIMIT_NULL,CSystem::GetScreenLeft(),CSystem::GetScreenTop(),CSystem::GetScreenRight(),CSystem::GetScreenBottom());	
// 播放挖金者的动作(恢复初始守候动作)
m_pGoldenMan->AnimateSpritePlayAnimation("GolderManAnimation2", 0);

m_iGoldCount=20;
//以下四个变量为金子能出现在地图上的上下左右边界最大值和最小值
	int		iGoldBornMinX	=	CSystem::GetScreenLeft() + 5;
	int		iGoldBornMaxX	=	CSystem::GetScreenRight() - 5;
	int		iGoldBornMinY	=	CSystem::GetScreenTop() + 20;
	int		iGoldBornMaxY	=   CSystem::GetScreenBottom() - 5;
	int		iLoop;
	int		iSize	, iPosX , iPosY ;
	char* 	tmpBlock;
	//钩子可以发出碰撞
	m_pGoldenHook->SetSpriteCollisionSend(true);
	//金子可以接收碰撞
	m_pGoldenTemplate->SetSpriteCollisionReceive(true);
	for( iLoop = 0; iLoop < m_iGoldCount; iLoop++ )
{
		if( iLoop < 10 )
		{
			iSize	=	4;
		}
		else if(iLoop < 16 )
		{
			iSize	=	6;
		}
		else
		{
			iSize	=	8;
		}
			int shangxia = rand()%(iGoldBornMaxY-iGoldBornMinY)+1;
			int zuoyou = rand()%(iGoldBornMaxX-iGoldBornMinX)+1;
		// 初始化金子精灵实例
		tmpBlock=CSystem::MakeSpriteName("GoldBlock",iLoop);
		CSprite *tmpGlodenBlock=new CSprite(tmpBlock);
		tmpGlodenBlock->CloneSprite("GoldTemplate");
		tmpGlodenBlock->SetSpriteWidth((float)iSize );
		tmpGlodenBlock->SetSpriteHeight((float)iSize);
		// 随机一个位置
				iPosX	=iGoldBornMinX+zuoyou;
				iPosY	=iGoldBornMinY+shangxia;
		tmpGlodenBlock->SetSpritePosition((float)iPosX, (float)iPosY);
		m_vGoldenBlock.push_back(tmpGlodenBlock);   //存放金子对象的指针
}
}
	
	
//=============================================================================
//
// 每局游戏进行中
void CGameMain::GameRun( float fDeltaTime )
{
	const	float	fRotateSpeed	=	45.f; // 摇摆速度，单位度/秒
float	 fThisRotate	=	fRotateSpeed * fDeltaTime; // 本次旋转的度数
// 如果当前为等待抓取状态，那么，左右摇摆钩子
if( 0 == m_iGetGoldState )
{	
	if( m_iHookRotToLeft )
	{
		m_fHookRotation	+= fThisRotate;
		if( m_fHookRotation >= 180.f )
		{
			m_fHookRotation		=	180.f;
			m_iHookRotToLeft	=	0;
		}
	}
	else
{
	m_fHookRotation	-= fThisRotate;
	if( m_fHookRotation <= 0.f )
	{
		m_fHookRotation		=	0.f;
		m_iHookRotToLeft	=	1;
	}
}
	m_pGoldenHook->SetSpriteRotation(m_fHookRotation);
	m_pGoldenMan->AnimateSpritePlayAnimation("GolderManAnimation2",0);
}
else if(3 == m_iGetGoldState )
{
		// 判断是否移动到终点(判断的依据是XY方向的移动速度为0，
      // 即金子是否已经停止移动)
		float	fSpeedX	=	m_pGoldenHook->GetSpriteLinearVelocityX();
		float	fSpeedY	=	m_pGoldenHook->GetSpriteLinearVelocityY();
		// 当前速度不为0，还在运动中
		// 浮点数是否为0，不能直接判断 == != 0
		if( fSpeedX > 0.00001f || fSpeedX < -0.00001f || fSpeedY > 0.00001f || fSpeedY < -0.00001f )
			return;
		// 速度为0，钩子回到初始点，开始下一轮的抓取
	    // 当前抓取到金子，将抓取到的金子释放并删除。然后将金子数量减一
		m_iGoldCount--;
m_CurGetGold->SpriteDismount();
			     m_CurGetGold->DeleteSprite();
				 m_iGetGoldState	=	0;
m_pGoldenMan->AnimateSpritePlayAnimation("GolderManAnimation2", 0);
}
else if(2 == m_iGetGoldState )
{
// 判断是否移动到终点(判断的依据是XY方向的移动速度为0，
//即金子是否已经停止移动)
float	fSpeedX	=	m_pGoldenHook->GetSpriteLinearVelocityX();
float	fSpeedY	=	m_pGoldenHook->GetSpriteLinearVelocityY();
// 当前速度不为0，还在运动中
// 浮点数是否为0，不能直接判断 == != 0
if( fSpeedX > 0.00001f || fSpeedX < -0.00001f || fSpeedY > 0.00001f || fSpeedY < -0.00001f )
return;		
m_iGetGoldState	=	0;	
}
}

//=============================================================================
//
// 本局游戏结束
void CGameMain::GameEnd()
{
	m_pGameBegin->SetSpriteVisible(1);
	m_pGoldenHook->SetSpriteRotation(0);
}
//==========================================================================
//
// 鼠标移动
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void CGameMain::OnMouseMove( const float fMouseX, const float fMouseY )
{
	
}
//==========================================================================
//
// 鼠标点击
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void CGameMain::OnMouseClick( const int iMouseType, const float fMouseX, const float fMouseY )
{
	
}
//==========================================================================
//
// 鼠标弹起
// 参数 iMouseType：鼠标按键值，见 enum MouseTypes 定义
// 参数 fMouseX, fMouseY：为鼠标当前坐标
void CGameMain::OnMouseUp( const int iMouseType, const float fMouseX, const float fMouseY )
{
	
}
//==========================================================================
//
// 键盘按下
// 参数 iKey：被按下的键，值见 enum KeyCodes 宏定义
// 参数 iAltPress, iShiftPress，iCtrlPress：键盘上的功能键Alt，Ctrl，Shift当前是否也处于按下状态(0未按下，1按下)
void CGameMain::OnKeyDown( const int iKey, const bool bAltPress, const bool bShiftPress, const bool bCtrlPress )
{	
if( KEY_SPACE == iKey && 0 == m_iGameState )
{
m_iGameState	=	1;
m_pGameBegin->SetSpriteVisible(0);
}
// 当前处于游戏进行中，按下向下的方向键，钩子伸出抓取金子
else if( KEY_DOWN == iKey && 2 == m_iGameState && 0 == m_iGetGoldState )
{
		// 设置抓取状态为：钩子往外伸
		m_iGetGoldState	=	1;
		// 以当前朝向给钩子一个向前的速度
		m_pGoldenHook->SetSpriteLinearVelocityPolar(m_fEmptyHookSpeed, m_fHookRotation);
		// 播放挖金者的动作(一个胳膊往下压的动作)
		m_pGoldenMan->AnimateSpritePlayAnimation("GolderManAnimation1", 0);
}
}
//==========================================================================
//
// 键盘弹起
// 参数 iKey：弹起的键，值见 enum KeyCodes 宏定义
void CGameMain::OnKeyUp( const int iKey )
{
	
}
//===========================================================================
//
// 精灵与精灵碰撞
// 参数 szSrcName：发起碰撞的精灵名字
// 参数 szTarName：被碰撞的精灵名字
void CGameMain::OnSpriteColSprite( const char *szSrcName, const char *szTarName )
{
if( 2 == m_iGameState )
{
if(1 == m_iGetGoldState && stricmp(szSrcName, "goldhook") == 0 && strstr( szTarName, "GoldBlock") != NULL )
{
	//到容器中找到金块对应的实例
	m_CurGetGold= GetCurGoldenSprite(szTarName);
	m_CurGetGold->SpriteMountToSpriteLinkPoint("GoldHook",2);
	float	fWidth  =	m_CurGetGold->GetSpriteWidth();
	m_iGetGoldState	=	3;
	float	fSpeed	=	((10.f - fWidth) / 10.f) * m_fEmptyHookSpeed;
	// 钩子往初始位置移动
m_pGoldenHook->SpriteMoveTo(m_fHookStartPosX, m_fHookStartPosY, fSpeed, 1);
// 播放挖金者的动作(胳膊来回动的动作)
	m_pGoldenMan->AnimateSpritePlayAnimation("GolderManAnimation3", 0 );
}
}	
}
//===========================================================================
//
// 精灵与世界边界碰撞
// 参数 szName：碰撞到边界的精灵名字
// 参数 iColSide：碰撞到的边界 0 左边，1 右边，2 上边，3 下边
void CGameMain::OnSpriteColWorldLimit( const char *szName, const int iColSide )
{
	//判断是否是钩子碰到边界，往回收
if(  stricmp( szName, "GoldHook" ) == 0 )
{
		// 设置抓取状态为：钩子往回收，未抓取到东西
		m_iGetGoldState	=	2;
		// 钩子往初始位置移动
		m_pGoldenHook->SpriteMoveTo(m_fHookStartPosX, m_fHookStartPosY, m_fEmptyHookSpeed, 1);
		// 播放挖金者的动作(胳膊来回动的动作)
		m_pGoldenMan->AnimateSpritePlayAnimation("GolderManAnimation3", 0);
}
}
void CGameMain::DrawHookLine()
{
	// 首先，从矿工精灵上获取一个缆绳链接点作为绳子的起始点(该链接点在编辑器
//里编辑好)
float	fStartX	=	m_pGoldenMan->GetSpriteLinkPointPosX(1);
float	fStartY	=	m_pGoldenMan->GetSpriteLinkPointPosY(1);
// 绳子终点在钩子精灵上获取(该链接点在编辑器里编辑好)
float	fEndX	=	m_pGoldenHook->GetSpriteLinkPointPosX(1);
float	fEndY	=	m_pGoldenHook->GetSpriteLinkPointPosY(1);
// 在这两点之间划线.线的颜色红绿蓝值都为50，即灰色
CSystem::DrawLine( fStartX, fStartY, fEndX, fEndY, 2.f, 0, 50, 50, 50, 255 );
}

CSprite* CGameMain::GetCurGoldenSprite(const char* goldenname)
{
	for(int i=0;i<m_vGoldenBlock.size();i++){
		if(strcmp(goldenname,m_vGoldenBlock[i]->GetName())==0){
			return m_vGoldenBlock[i];
		}
	}
	return NULL;
}
//**************************************************************************/
// Copyright (c) 1998-2007 Autodesk, Inc.
// All rights reserved.
// 
// These coded instructions, statements, and computer programs contain
// unpublished proprietary information written by Autodesk, Inc., and are
// protected by Federal copyright law. They may not be disclosed to third
// parties or copied or duplicated in any form, in whole or in part, without
// the prior written consent of Autodesk, Inc.
//**************************************************************************/
// DESCRIPTION: Appwizard generated plugin
// AUTHOR: 
//***************************************************************************/

#include "stdafx.h"
#include "Red3DMaxEffectUtil2008.h"
#include "RMaxDataHelper.h"
#include "LogFilePort.h"

#define Red3DMaxEffectUtil2008_CLASS_ID	Class_ID(0xc996058b, 0x7d7137bd)		/*!< ������ؼ���ID */	

const int MAX_NODE					= 50;										/*!< һ���ڵ������50��ͼԪ */

/*!	\class Red3DMaxEffectUtil2008
 *	\brief ��Ч�������
 */
class Red3DMaxEffectUtil2008 : public UtilityObj 
{
	/*!	\class REffectVertex
	 *	\brief ��Ч�������
	 */
	class REffectVertex
	{
	public:
		Point3			m_vecPosition;						/*!< λ�� */
		Point2			m_vecTextureCoordinate;				/*!< ��������0-1��Χ�� */
		color			m_vecNormal;						/*!< ���߷��� */
		color			m_vecTangentU;						/*!< ����U */
		color			m_vecTangentV;						/*!< ����V */
	};

	/*!	\class RTextureMat
	 *	\brief ������ʽṹ
	 */
	class RTextureMat
	{
	public:		
		string			m_strTextureName;					/*!< �������� */
		int				m_nMatID;							/*!< ����ID */
		int				m_nMatSubID;						/*!< �Ӳ���ID */
	};	

	/*!	\class RMeshAnimation
	 *	\brief ģ�Ͷ����ṹ
	 */
	class RMeshAnimation
	{
	public:		
		vector< Point3 > m_lstPosition;						/*!< λ����Ϣ */
		vector< Point2 > m_lstTextureCoord;					/*!< �������� */
		vector< color >	m_lstColor;							/*!< ��ɫ���� */
		vector< color >	m_lstNormal;						/*!< ���߶��� */
		vector< color >	m_lstTangentU;						/*!< ����U�б� */
		vector< color >	m_lstTangentV;						/*!< ����V�б� */
	};

	/*!	\struct RWeight
	 *	\brief ����Ȩ��
	 */
	struct RWeight
	{
		vector< string >	vecBoneName;					/*!< �������� */
		vector< int >		vecBoneIndex;					/*!< �������� */
		vector< float >		vecWeight;						/*!< ����Ȩ�� */
	};

	/*!	\class RMesh
	 *	\brief ģ������
	 */
	class RMesh
	{
	public:
		INode			*m_pNode;							/*!< �ڵ�ָ�� */
		INode			*m_pParent;							/*!< ���ڵ�ָ�� */

		int				m_nNode;							/*!< �ڵ����� */
		int				m_nParent;							/*!< ���ڵ����� */
		int				m_nVertexCount;						/*!< ������� */
		int				m_nIndexCount;						/*!< �������� */
		int				m_nAnimationType;					/*!< �������� */
		int				m_nBillboardType;					/*!< ��������� */
		bool			m_bMeshAnimation;					/*!< �Ƿ���ģ�Ͷ��� */

		char			m_szName[ 64 ];						/*!< ģ���������� */

		Matrix3			m_matWorld;							/*!< ������� */
		Matrix3			m_matLocal;							/*!< ���ؾ��� */
		Matrix3			m_matInvWorld;						/*!< ��������� */

		vector< int >	m_lstTextureID;						/*!< ����ID�б� */
		vector< int >	m_lstVertexCount;					/*!< ��������б� */
		vector< int >	m_lstVertexOffset;					/*!< ����ƫ���б� */
		vector< int >	m_lstIndexCount;					/*!< ���������б� */
		vector< int >	m_lstIndexOffset;					/*!< ����ƫ���б� */

		vector< Point3 > m_lstPosition;						/*!< λ���б� */
		vector< Point2 > m_lstTextureCoord;					/*!< ���������б� */
		vector< color >	m_lstColor;							/*!< ��ɫ�б� */
		vector< color >	m_lstNormal;						/*!< �����б� */
		vector< color >	m_lstTangentU;						/*!< ����U�б� */
		vector< color >	m_lstTangentV;						/*!< ����V�б� */

		vector< WORD >	m_lstIndex;							/*!< �����б� */

		vector< Point3 >	m_lstAnimationPositionKey;		/*!< �����ؼ�֡λ���б� */
		vector< Point4 > m_lstAnimationRotateKey;			/*!< �����ؼ�֡��ת�б� */
		vector< Point3 >	m_lstAnimationScaleKey;			/*!< �����ؼ�֡�����б� */
		vector< float >	m_lstAnimationVisibilityKey;		/*!< �����ؼ�֡�ɼ����б� */
		
		vector< RMeshAnimation* >	m_lstMeshAnimation;		/*!< ���񶯻��б� */

		int					m_iUsedBoneCount;				/*!< ʹ�ù������� */
		vector< RWeight >	m_vecWeightList;				/*!< Ȩ��ֵ�б� */

		RMesh() : m_iUsedBoneCount(0) { m_bMeshAnimation = false; }
		~RMesh() {
			// ��������񶯻�,��������ͷ��ڴ�
			if( m_bMeshAnimation )
			{
				for( UINT cAnimation = 0; cAnimation < m_lstMeshAnimation.size(); cAnimation++ )
				{
					delete m_lstMeshAnimation[ cAnimation ];
				}
			}
		}

	};

	/*!	\struct BoneData
	 *	\brief ��������
	 */
	struct BoneData
	{
		BoneData() : pNode(NULL), iParentID(-1) {}
		string				strBoneName;					/*!< �������� */
		int					iParentID;						/*!< ������ID */
		INode *				pNode;							/*!< ָ��Objectָ�� */

		Matrix3				matWorld;						/*!< ������� */
		Matrix3				matLocal;						/*!< �ֲ����� */

		vector< Point3 >	vecPositionList;				/*!< λ���б� */
		vector< Point4 >	vecRotationList;				/*!< ��ת�б� */
		vector< Point3 >	vecScaleList;					/*!< �����б� */
	};

	vector< Mtl* >		m_lstMAXMatrial;					/*!< �����б� */
	vector< RTextureMat > m_lstTexture;						/*!< �����б�,����������ṹ */

	vector< RMesh* >	m_lstMesh;							/*!< ģ���б� */
	vector< string >	m_lstSaveTextureName;				/*!< ��������ͼ�����б� */
	vector< Point3 >	m_lstSavePosition[ MAX_NODE ];		/*!< ͼԪλ����Ϣ */
	vector< Point3 >	m_lstSaveNormal[ MAX_NODE ];		/*!< ͼԪ������Ϣ */
	vector< Point2 >	m_lstSaveTexture[ MAX_NODE ];		/*!< ͼԪ����������Ϣ */
    vector< color >		m_lstSaveColor[ MAX_NODE ];			/*!< ͼԪ��ɫ��Ϣ */
	vector< WORD >		m_lstSaveIndex[ MAX_NODE ];			/*!< ͼԪ����������Ϣ */
	vector< RWeight >	m_lstSaveWeight[ MAX_NODE ];		/*!< ͼԪȨ����Ϣ */

	// ����������־�˶������������
    vector< Point3 >	m_lstFinalPosition;
	vector< Point3 >	m_lstFinalNormal;
	vector< Point2 >	m_lstFinalTexture;
	vector< color >		m_lstFinalColor;
	vector< WORD >		m_lstFinalIndex;
	vector< string >	m_lstFinalTextureName;				/*!< �����������ͼ */

	int					m_nFirstFrame;						/*!< ��һ֡ */
	int					m_nLastFrame;						/*!< ���֡ */
	int					m_nFrameSpeed;						/*!< ֡�� */
	int					m_nTicksPerFrame;					/*!< ֡�� */

	bool				m_bVertexAni;						/*!< �Ƿ��Ƕ��㶯�� */

	vector< BoneData * >		m_vecBoneList;				/*!< �����б� */
public:
		
	//Constructor/Destructor
	Red3DMaxEffectUtil2008();
	virtual ~Red3DMaxEffectUtil2008();

	virtual void DeleteThis() { };		
	
	virtual void BeginEditParams(Interface *ip,IUtil *iu);
	virtual void EndEditParams(Interface *ip,IUtil *iu);

	virtual void Init(HWND hWnd);							/*!< ��ʼ�� */
	virtual void Destroy(HWND hWnd);						/*!< ���� */

	void				Cleanup();							/*!< ��� */
	void				Export();							/*!< ��� */

private:

	static INT_PTR CALLBACK DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	HWND			hPanel;
	IUtil			*iu;
	Interface		*ip;

	/*!	\fn MakeMaterialList
	 *	\param pNode �ڵ�ָ��
	 *	\brief �������ʱ�
	 */
	bool				MakeMaterialList( INode *pNode );

	/*!	\fn MakeTextureList
	 *	\brief ���������б�
	 */
	bool				MakeTextureList(void);

	/*!	\fn ExportMesh
	 *	\param pNode �ڵ�ָ��
	 *	\brief �������ģ��
	 */
	void				ExportMesh( INode *pNode );

	/*!	\fn GetMeshID
	 *	\param pNode �ڵ�ָ��
	 *	\brief ��ȡ����ģ��ID
	 */
	int					GetMeshID( INode *pNode );

	/*!	\fn MakeR3DMesh
	 *	\param pMesh ����ģ��
	 *	\brief ����R3D����ģ��
	 */
	void				MakeR3DMesh( RMesh *pMesh );

	/*!	\fn MakeR3DMeshAnimation
	 *	\param pMesh ���񶯻�ģ��
	 *	\brief ����R3D����ģ�Ͷ���
	 */
	void				MakeR3DMeshAnimation( RMeshAnimation *pMesh );

	/*!	\fn MakeSaveVertex
	 *	\param pPosition ����λ�ö���
	 *	\param pNormal ���߶���
	 *	\param pTexture �������
	 *	\param pColor ��ɫ����
	 *	\param pWeight Ȩ�ض���
	 *	\param pMtl ���ʶ���
 	 *	\param nSubMat �Ӳ���ID
	 *	\brief ���涥����Ϣ
	 */
	bool				MakeSaveVertex( Point3 *pPosition, Point3 *pNormal, Point2 *pTexture, color *pColor, RWeight * pWeight, Mtl *pMtl, int nSubMat );

	/*!	\fn ClearSaveBuffers
	 *	\brief ������滺��
	 */
	void				ClearSaveBuffers(void);

	/*!	\fn ExportAnimationElement
	 *	\brief �������Ԫ��
	 */
	void				ExportAnimationElement(void);

	/*!	\fn ExportAnimationMesh
	 *	\param pNode �ڵ�ָ��
	 *	\param t ��ǰ�ڵ��ʱ�����
 	 *	\param iAniType ��������
	 *	\brief �������Ԫ������
	 */
	void				ExportAnimationMesh( INode *pNode, TimeValue t, int iAniType );
	
	/*!	\fn MakeAnimationList
	 *	\brief ���������б�
	 */
	void				MakeAnimationList(void);

	/*!	\fn MakeGeometry
	 *	\brief ����ͼԪ
	 */
	bool				MakeGeometry(void);

	/*!	\fn FillNodeList
	 *	\param pNode �ڵ�ָ��
 	 *	\param lstNodeList �ڵ����
	 *	\brief ���ڵ��б�
	 */
	void				FillNodeList( INode *pNode, vector< INode* > &lstNodeList );

	/*!	\fn SaveEffectFile
	 *	\param strFilename Ч���ļ���
	 *	\brief ����Ч���ļ�
	 */
	void				SaveEffectFile( string strFilename );

	/*!	\fn GetSaveFileNamePath
	 *	\brief ��ȡ�����ļ�·��
	 */
	string				GetSaveFileNamePath(void);

	/*!	\fn SetVertexWeightList
	 *	\param vecWeightList Ȩ���б�
 	 *	\param pNode �ڵ�ָ��
	 *	\brief ���涥��Ȩ���б�
	 */
	int					SetVertexWeightList( vector< RWeight > & vecWeightList, INode * pNode );

	/*!	\fn GetPhysiqueMod
	 *	\param pNode �ڵ�ָ��
 	 *	\return �޸���ָ��
	 *	\brief ��ȡ�޸���ָ��
	 */
	Modifier *			GetPhysiqueMod( INode *pNode );

	/*!	\fn GetBoneID
	 *	\param pNode �ڵ�ָ��
 	 *	\return ��ȡ��ǰ�ڵ�Ĺ���ID
	 *	\brief ��ȡ����ID
	 */
	int					GetBoneID( INode * pNode );

	/*!	\fn GetSkinMod
	 *	\param pNode �ڵ�ָ��
 	 *	\return Ƥ���޸���ָ��
	 *	\brief ��ȡƤ���޸���
	 */
	Modifier *			GetSkinMod( INode * pNode );

	/*!	\fn IsBone
	 *	\param pNode �ڵ�ָ��
	 *	\brief �Ƿ��ǹ����ڵ�
	 */
	BOOL				IsBone(INode* pNode);

	/*!	\fn ExportBone
	 *	\param pNode �ڵ�ָ��
	 *	\brief �������
	 */
	void				ExportBone( INode *pNode );

	/*!	\fn SetAnimationKeyList
	 *	\param pBoneData ��������ָ��
	 *	\param pNode �ڵ�ָ��
	 *	\brief ���ùؼ������б�
	 */
	void				SetAnimationKeyList( BoneData * pBoneData, INode * pNode );

	//////////////////////////////////////////////////////////////////////////
	// д��XML�ļ�
	void r3dWriteFloatMatrixToXML( FILE *fp, float *fMatrix );
	void r3dWriteEffectTextureToXML( FILE *fp, vector< string > &lstTexture );
	bool r3dWriteEffectMeshToXML( FILE *fp, vector< Red3DMaxEffectUtil2008::RMesh* > &lstMesh );
	void r3dWriteEffectBoneToXML(FILE *fp, vector< BoneData * > &lstBone, bool bBoneType);
	void r3dWriteEffectXML( string strFileName );
	//////////////////////////////////////////////////////////////////////////
};

static Red3DMaxEffectUtil2008 theRed3DMaxEffectUtil2008;


class Red3DMaxEffectUtil2008ClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic() 							{ return TRUE; }
	virtual void* Create(BOOL /*loading = FALSE*/) 	{ return &theRed3DMaxEffectUtil2008; }
	virtual const TCHAR *	ClassName() 			{ return GetString(IDS_CLASS_NAME); }
	virtual SClass_ID SuperClassID() 				{ return UTILITY_CLASS_ID; }
	virtual Class_ID ClassID() 						{ return Red3DMaxEffectUtil2008_CLASS_ID; }
	virtual const TCHAR* Category() 				{ return GetString(IDS_CATEGORY); }

	virtual const TCHAR* InternalName() 			{ return _T("Red3DMaxEffectUtil2008"); }	// returns fixed parsable name (scripter-visible name)
	virtual HINSTANCE HInstance() 					{ return hInstance; }					// returns owning module handle
	

};

static Red3DMaxEffectUtil2008ClassDesc Red3DMaxEffectUtil2008Desc;
ClassDesc2* GetRed3DMaxEffectUtil2008Desc() { return &Red3DMaxEffectUtil2008Desc; }

void OnCommand( HWND hWnd, WPARAM wParam, LPARAM lParam );


//--- Red3DMaxEffectUtil2008 -------------------------------------------------------
Red3DMaxEffectUtil2008::Red3DMaxEffectUtil2008() : m_bVertexAni(true)
{
	iu = NULL;
	ip = NULL;	
	hPanel = NULL;
}

Red3DMaxEffectUtil2008::~Red3DMaxEffectUtil2008()
{
	Cleanup();
}

void Red3DMaxEffectUtil2008::Cleanup()
{
	// ���ģ�͵������ڴ�
	if ( !m_lstMesh.empty() )
	{
		for ( int i = 0; i < (int) m_lstMesh.size(); ++i )
		{
			if ( m_lstMesh[ i ] )
				delete m_lstMesh[ i ];
		}
		m_lstMesh.clear();
	}

	// ��������б��ڴ�
	if ( !m_vecBoneList.empty() )
	{
		for ( int i = 0; i < (int) m_vecBoneList.size(); ++i )
		{
			if ( m_vecBoneList[ i ] )
				delete m_vecBoneList[ i ];
		}
		m_vecBoneList.clear();
	}
}

void Red3DMaxEffectUtil2008::BeginEditParams(Interface* ip,IUtil* iu) 
{
	this->iu = iu;
	this->ip = ip;
	hPanel = ip->AddRollupPage(
		hInstance,
		MAKEINTRESOURCE(IDD_PANEL),
		DlgProc,
		GetString(IDS_PARAMS),
		0);
}
	
void Red3DMaxEffectUtil2008::EndEditParams(Interface* ip,IUtil* iu) 
{
	this->iu = NULL;
	this->ip = NULL;
	ip->DeleteRollupPage(hPanel);
	hPanel = NULL;
}

void Red3DMaxEffectUtil2008::Init(HWND hWnd)
{
	int AssertMode = 0;
	int ErrortMode = 0;
	AssertMode |= _CRTDBG_MODE_DEBUG;
	ErrortMode |= _CRTDBG_MODE_DEBUG;
	AssertMode |= _CRTDBG_MODE_WNDW;
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode( _CRT_ASSERT, AssertMode );
	_CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode( _CRT_ERROR, ErrortMode );
	_CrtSetReportFile( _CRT_ERROR, _CRTDBG_FILE_STDERR);
	_CrtSetReportMode( _CRT_WARN, ErrortMode );
	_CrtSetReportFile( _CRT_WARN, _CRTDBG_FILE_STDERR);
}

void Red3DMaxEffectUtil2008::Destroy(HWND hWnd)
{

}

INT_PTR CALLBACK Red3DMaxEffectUtil2008::DlgProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg) 
	{
		case WM_INITDIALOG:
			theRed3DMaxEffectUtil2008.Init(hWnd);
			break;

		case WM_DESTROY:
			theRed3DMaxEffectUtil2008.Destroy(hWnd);
			break;

		case WM_COMMAND:
			OnCommand( hWnd, wParam, lParam );
			break;

		case WM_LBUTTONDOWN:
		case WM_LBUTTONUP:
		case WM_MOUSEMOVE:
			theRed3DMaxEffectUtil2008.ip->RollupMouseMessage(hWnd,msg,wParam,lParam); 
			break;

		default:
			return 0;
	}
	return 1;
}

void OnCommand( HWND hWnd, WPARAM wParam, LPARAM lParam )
{
	switch( LOWORD( wParam ) )
	{
		case IDC_DOEXPORT:
			{		
				/*! < Ψһ����� */
				theRed3DMaxEffectUtil2008.Export();				
			}
			break;
	}
}

/*!	\fn ComputeTangentVector
 *	\param vecPosA ��Aλ��
 *	\param vecPosB ��Bλ��
 *	\param vecPosC ��Cλ��
 *	\param vecTexA ��A����λ��
 *	\param vecTexB ��B����λ��
 *	\param vecTexC ��C����λ��
 *	\param vecS ��������U,���
 *	\param vecT ��������V,���
 *	\brief ������������
 */
bool ComputeTangentVector( Point3 vecPosA, Point3 vecPosB, Point3 vecPosC, Point2 vecTexA, Point2 vecTexB, Point2 vecTexC, Point3 *vecS, Point3 *vecT )	
{
	// ������������AB,AC
	Point3 vAB = vecPosB - vecPosA;
	Point3 vAC = vecPosC - vecPosA;

	// ������㷨�߷���
	Point3 vecNormal = ( vecPosB - vecPosA ) ^ ( vecPosC - vecPosB );
	vecNormal.Normalize();		// ���߱�׼��

	Point3 n = vecNormal;

	// ����n������AB�ĵ��
	Point3 vProjAB = vAB - ( ( n % vAB ) * n );
	Point3 vProjAC = vAC - ( ( n % vAC ) * n );

	// ������2ά�ռ����������
	float duAB = vecTexB.x - vecTexA.x;
	float duAC = vecTexC.x - vecTexA.x;

	float dvAB = vecTexB.y - vecTexA.y;
	float dvAC = vecTexC.y - vecTexA.y;

	bool bCheck = false;

	// ���������ĵ��, �������������ĵ��
	if( duAC * dvAB > duAB * dvAC )
	{
		//duAC = -duAC;
		//duAB = -duAB;

		bCheck = true;
	}
	
	// ��������
	Point3 vTangent = duAC * vProjAB - duAB * vProjAC;
	vTangent.Normalize();		// ����������׼��

	// ���
	if( bCheck )
	{
		vecS[ 0 ] = -vTangent;
		vecS[ 1 ] = -vTangent;
		vecS[ 2 ] = -vTangent;
	}
	else	
	{
		vecS[ 0 ] = vTangent;
		vecS[ 1 ] = vTangent;
		vecS[ 2 ] = vTangent;
	}

	/*
	if( duAC * dvAB > duAB * dvAC )
	{
		dvAC = -dvAC;
		dvAB = -dvAB;
	}
	*/

	vTangent = dvAC * vProjAB - dvAB * vProjAC;
	vTangent.Normalize();
	
	if( bCheck )
	{
		vecT[ 0 ] = -vTangent;
		vecT[ 1 ] = -vTangent;
		vecT[ 2 ] = -vTangent;
	}
	else	
	{
		vecT[ 0 ] = vTangent;
		vecT[ 1 ] = vTangent;
		vecT[ 2 ] = vTangent;
	}

	if( bCheck )
	{
		int a=0;
	}

	return bCheck;
}

/*!	\fn Export
 *	\brief ���
 */
void Red3DMaxEffectUtil2008::Export()
{
	Cleanup();

	m_lstMAXMatrial.resize( 0 );
	m_lstTexture.resize( 0 );

	m_lstMesh.resize( 0 );
	m_lstSaveTextureName.resize( 0 );

	m_lstFinalPosition.resize( 0 );
	m_lstFinalNormal.resize( 0 );
	m_lstFinalTexture.resize( 0 );
	m_lstFinalColor.resize( 0 );
	m_lstFinalIndex.resize( 0 );
	m_lstFinalTextureName.resize( 0 );

	// �������
	ClearSaveBuffers();
	
	string strFilename = GetSaveFileNamePath();
	if ( strFilename == "" )
		return;

	// ����ʱ�����
	Interval range = ip->GetAnimRange();									// �õ�������Χ
	m_nFirstFrame = ( int ) ( range.Start() / GetTicksPerFrame() );			// �õ���һ֡
	m_nLastFrame = ( int ) ( range.End() / GetTicksPerFrame() );			// �õ����һ֡
	m_nFrameSpeed = ( int ) ( GetFrameRate() );								// �õ�֡�ٶ�
	m_nTicksPerFrame = ( int ) ( GetTicksPerFrame() );						// �õ�ÿ֡������Tick

	// ����
	MakeMaterialList( ip->GetRootNode() );									// ��Ӳ���

	// ����
	MakeTextureList();														// ͨ�������������

	// ��ȡͼԪ
	if ( MakeGeometry() == NULL )
	{
		MessageBox( NULL, "Exportlist empty", "max2008", MB_OK );
		return;
	}
	// ����
	MakeAnimationList();

	// ����ΪЧ���ļ�
	SaveEffectFile( strFilename );	
}

bool Red3DMaxEffectUtil2008::MakeMaterialList( INode *pNode )
{
	Mtl *pMtl = pNode->GetMtl();

	if( pMtl )
	{
		AddUniqueVector(m_lstMAXMatrial, pMtl );								// �������ֻ����Ψһ
	}

	for( int cNode = 0; cNode < pNode->NumberOfChildren(); cNode++ )
		MakeMaterialList( pNode->GetChildNode( cNode ) );	

	return true;
}

bool Red3DMaxEffectUtil2008::MakeTextureList(void)
{
	m_lstFinalTextureName.resize( 0 );

	// �������ʱ�
	for( UINT cMat=0; cMat < m_lstMAXMatrial.size(); cMat++ )
	{
		RTextureMat AddNode;

		AddNode.m_nMatID = cMat;																		// ����ID
		AddNode.m_nMatSubID = -1;																		// ������ID
		AddNode.m_strTextureName = RHelper::GetDiffuseTextureMap( m_lstMAXMatrial[ cMat ] );			// ��ȡ����·��	

		// ����ڵ������ͼ��Ϊ����ѹ��
		if( !( AddNode.m_strTextureName == "" ) )
			m_lstTexture.push_back( AddNode );

		// ��ȡ�Ӳ�������
		int nSubMat = m_lstMAXMatrial[ cMat ]->NumSubMtls();
		for( int cSubMat=0; cSubMat < nSubMat; cSubMat++ )
		{
			AddNode.m_nMatID = cMat;
			AddNode.m_nMatSubID = cSubMat;
			AddNode.m_strTextureName = RHelper::GetDiffuseTextureMap( m_lstMAXMatrial[ cMat ]->GetSubMtl( cSubMat ) );
			if( !( AddNode.m_strTextureName == "" ) )
				m_lstTexture.push_back( AddNode );
		}
	}

	// ֻ��������ͼ��·��
	for( UINT cTexture = 0; cTexture < m_lstTexture.size(); cTexture++ )
		m_lstFinalTextureName.push_back( m_lstTexture[ cTexture ].m_strTextureName );
	return true;	
}

// ����������Ϣ
void Red3DMaxEffectUtil2008::ExportMesh( INode *pNode )
{
	// ����ǹ������˳��˽ڵ�
	if ( IsBone( pNode ) )
		return;

	char * szName = pNode->GetName();

	// ͼԪ�������в�����"Nub"��"nub"���ַ���
	if ( strstr( szName, "Nub" ) != NULL || strstr( szName, "nub" ) != NULL )
		return;

	// ��ȡ����ָ��,��������ڲ�����ֱ���˳�
	// �������ЧƤ���ڵ�������в���
	Mtl *pMtl = pNode->GetMtl();
	if( pMtl == NULL )
		return;	

	// ȥ��ǰͼԪ�Ƿ�Ϊ��
	int v1, v2, v3;	
	bool bNegScale = RHelper::TMNegParity( pNode->GetObjectTM( 0 ) );

	// ��������ı�
	if( bNegScale )
	{
		v1 = 0; v2 = 1; v3 = 2;
	}
	else
	{
		v1 = 2; v2 = 1; v3 = 0;
	}

	bool bNeedDel;

	// �Ӷ����ȡͼԪ
	TriObject *pTri = RHelper::GetTriObjectFromNode( pNode, 0, bNeedDel );
	if( pTri == NULL )	
		return;

	// ��ͼԪ�����ȡģ��
	Mesh *pMesh = &pTri->GetMesh();
	pMesh->buildNormals();								// ���㶥�����淨��

	INode *pParent = pNode->GetParentNode();			// ��ȡ����ĸ�����
	INode *pRoot = ip->GetRootNode();					// ��ȡ���ڵ�

	// ����һ��RMesh�ڴ�
	RMesh *pMeshObject = new RMesh;
	pMeshObject->m_nNode = (int)m_lstMesh.size();		// ģ�͵�����
	pMeshObject->m_pNode = pNode;						// ����ڵ�ָ��

	strcpy( pMeshObject->m_szName, pNode->GetName() );	// ģ�͵�����

	// �丸�ڵ㲻���ڸ��ڵ��ȡ���ڵ��ID
	if( pParent != pRoot )
		pMeshObject->m_nParent = GetMeshID( pNode->GetParentNode() );
	else	// ����丸�ڵ��Ǹ��ڵ������ø�IDΪ-1
		pMeshObject->m_nParent = -1;	

	// ������ڵ㲻Ϊ�ղ��Ҹ��ڵ㲢�Ǹ��ڵ�
	// �˽ڵ�ľֲ����� = ��ǰ�ڵ��������� * ���ڵ������� = ��ǰ�ڵ��ڸ��ռ�ľ���
	if( pParent && !pParent->IsRootNode())
		RHelper::MAXtoD3D( pNode->GetNodeTM( 0 ) * Inverse( pNode->GetParentTM( 0 ) ), pMeshObject->m_matLocal );
	else	// ���������ڸ��ڵ����,��ģ�ͽڵ�ľ���Ϊ���ľֲ�����
		RHelper::MAXtoD3D( pNode->GetNodeTM( 0 ), pMeshObject->m_matLocal  );
	// ģ�͵��������
	RHelper::MAXtoD3D( pNode->GetNodeTM( 0 ), pMeshObject->m_matWorld  );
	
	pMeshObject->m_nBillboardType = RHelper::GetBillboardType( pMeshObject->m_szName );				// ��ȡ���������
	pMeshObject->m_nAnimationType = RHelper::GetAnimationType( pMeshObject->m_szName );				// ��ȡ��������
	
	vector< Point3 > vecPositionList;
	vector< Point2 > vecTextureList;
	vector< Point3 > vecNormalList;
	vector< color > clrColorList;
	vector< DWORD > vecFaceList;
	vector< DWORD > vecTFaceList;
	vector< WORD > vecMatList;
	vector< DWORD > vecCFaceList;

	int nVertex	= pMesh->getNumVerts();			// ���������
	int nFace = pMesh->getNumFaces();			// ͼԪ������
	int nTexture = pMesh->getNumTVerts();		// ���������
	int nColor = pMesh->numCVerts;				// ���ص������

	Point3 p3Position, p3Normal, p3Texture, p3Color;
	Point3 vecPosition, vecNormal;
	Point2 vecTexture;	

	Matrix3 mtWorld = pNode->GetObjectTM( 0 );	// �ڵ���������
	Matrix3 mtInvTM = Inverse( mtWorld );		// ����������
	
	// ÿ�������λ��
	for( int cVertex = 0; cVertex < nVertex; cVertex++ )
	{
		// �����ÿ�����㶼�����������ֲ�����ϵ������
		// ����ǹ�������,����������ҳ�ģ�͵Ķ����������㵽����ռ���
		if ( pMeshObject->m_nAnimationType & REAT_BONE )
			vecPosition = RHelper::MAXtoR3D( mtWorld * pMesh->verts[ cVertex ] );
		// ģ�Ͷ�������Ҫ��¼���������ռ��λ����Ϣ
		else
			vecPosition = RHelper::MAXtoR3D( pMesh->verts[ cVertex ] );

		// ѹ��λ�ö���
		vecPositionList.push_back( vecPosition );
	}

	// �����UV����
	for( int cTexture=0; cTexture < nTexture; cTexture++ )
	{
		p3Texture = pMesh->tVerts[ cTexture ];
		vecTexture.x = p3Texture.x;
		vecTexture.y = 1.0f - p3Texture.y;				// ΪʲôҪ1��ȥ��,������֪�����ֵ�ķ�Χ�ض���0-1֮��
		vecTextureList.push_back( vecTexture );
	}


	// ��������
	for( int cColor=0; cColor < nColor; cColor++ )
	{
		p3Color = pMesh->vertCol[ cColor ];						// ��ȡһ������
		color clr;
		clr.a = 0x0;											// û��͸��Ч��
		clr.r = ( unsigned char )( p3Color.x * 255.0f );
		clr.g = ( unsigned char )( p3Color.y * 255.0f );
		clr.b = ( unsigned char )( p3Color.z * 255.0f );

		clrColorList.push_back( clr );
	}

	// ͼԪ����
	for( int cFace = 0; cFace < nFace; cFace++ )					
	{
		// ѹ��ÿ������������������,�ټ��ϵ�ǰ��Ĳ���������
		vecFaceList.push_back( pMesh->faces[ cFace ].v[ v1 ] );
		vecFaceList.push_back( pMesh->faces[ cFace ].v[ v2 ] );
		vecFaceList.push_back( pMesh->faces[ cFace ].v[ v3 ] );
		vecMatList.push_back( pMesh->faces[ cFace ].getMatID() );				

		// �����������������
		vecTFaceList.push_back( pMesh->tvFace[ cFace ].t[ v1 ] );
		vecTFaceList.push_back( pMesh->tvFace[ cFace ].t[ v2 ] );
		vecTFaceList.push_back( pMesh->tvFace[ cFace ].t[ v3 ] );		

		// ѹ�붥��ɫ
		if( nColor > 0 )
		{
			vecCFaceList.push_back( pMesh->vcFace[ cFace ].t[ v1 ] );
			vecCFaceList.push_back( pMesh->vcFace[ cFace ].t[ v2 ] );
			vecCFaceList.push_back( pMesh->vcFace[ cFace ].t[ v3 ] );
		}
	}

	// ����ǹ����������¼Ȩ��
	vector< RWeight > vecWeightList;
	int iUsedWeightCount = 0;
	// ����ǹ�������
	if ( pMeshObject->m_nAnimationType & REAT_BONE )
		iUsedWeightCount = SetVertexWeightList( vecWeightList, pNode );			// ���ж����ܹ���������

	Point3 vecSavePosition[ 3 ], vecSaveNormal[ 3 ], vecSaveTangentU[ 3 ], vecSaveTangentV[ 3 ];
	Point2 vecSaveTexture[ 3 ];

	color clrSaveColor[ 3 ];

	RWeight	weight[ 3 ];

	mtWorld.NoTrans();

	// ��������ڵ������ͼԪ
	for( int cFace=0; cFace < nFace ; cFace++ )
	{	
		// ��ȡͼԪ��3������λ��
		vecSavePosition[ 0 ] = vecPositionList[ vecFaceList[ cFace * 3 + 0 ] ];
		vecSavePosition[ 1 ] = vecPositionList[ vecFaceList[ cFace * 3 + 1 ] ];
		vecSavePosition[ 2 ] = vecPositionList[ vecFaceList[ cFace * 3 + 2 ] ];

		// ��ȡͼԪ��3���������������
		vecSaveTexture[ 0 ] = vecTextureList[ vecTFaceList[ cFace * 3 + 0 ] ];
		vecSaveTexture[ 1 ] = vecTextureList[ vecTFaceList[ cFace * 3 + 1 ] ];
		vecSaveTexture[ 2 ] = vecTextureList[ vecTFaceList[ cFace * 3 + 2 ] ];

		// ��ȡͼԪ��3�������Ȩ��
		if ( !vecWeightList.empty() )
		{
			weight[ 0 ] = vecWeightList[ vecFaceList[ cFace * 3 + 0 ] ];
			weight[ 1 ] = vecWeightList[ vecFaceList[ cFace * 3 + 1 ] ];
			weight[ 2 ] = vecWeightList[ vecFaceList[ cFace * 3 + 2 ] ];
		}

		Face* f;
		f = &pMesh->faces[ cFace ];	

		// ��ȡͼԪ��3������ķ���
		vecSaveNormal[ 0 ] = RHelper::GetVertexNormal( pMesh, cFace, pMesh->getRVertPtr( f->getVert( v1 ) ) );
		vecSaveNormal[ 1 ] = RHelper::GetVertexNormal( pMesh, cFace, pMesh->getRVertPtr( f->getVert( v2 ) ) );
		vecSaveNormal[ 2 ] = RHelper::GetVertexNormal( pMesh, cFace, pMesh->getRVertPtr( f->getVert( v3 ) ) );

		Point3 vNormal[ 3 ];

		// ������������
		vNormal[ 0 ].x = vecSaveNormal[ 0 ].x;
		vNormal[ 0 ].y = vecSaveNormal[ 0 ].y;
		vNormal[ 0 ].z = vecSaveNormal[ 0 ].z;

		vNormal[ 1 ].x = vecSaveNormal[ 1 ].x;
		vNormal[ 1 ].y = vecSaveNormal[ 1 ].y;
		vNormal[ 1 ].z = vecSaveNormal[ 1 ].z;

		vNormal[ 2 ].x = vecSaveNormal[ 2 ].x;
		vNormal[ 2 ].y = vecSaveNormal[ 2 ].y;
		vNormal[ 2 ].z = vecSaveNormal[ 2 ].z;

		//Point3 vTransNormal[ 3 ];

		vecSaveNormal[ 0 ] = RHelper::MAXtoR3D( vNormal[ 0 ] );
		vecSaveNormal[ 1 ] = RHelper::MAXtoR3D( vNormal[ 1 ] );
		vecSaveNormal[ 2 ] = RHelper::MAXtoR3D( vNormal[ 2 ] );

		//vecSaveNormal[ 0 ].x = vTransNormal[ 0 ].x;
		//vecSaveNormal[ 0 ].y = vTransNormal[ 0 ].z;
		//vecSaveNormal[ 0 ].z = vTransNormal[ 0 ].y;

		//vecSaveNormal[ 1 ].x = vTransNormal[ 1 ].x;
		//vecSaveNormal[ 1 ].y = vTransNormal[ 1 ].z;
		//vecSaveNormal[ 1 ].z = vTransNormal[ 1 ].y;

		//vecSaveNormal[ 2 ].x = vTransNormal[ 2 ].x;
		//vecSaveNormal[ 2 ].y = vTransNormal[ 2 ].z;
		//vecSaveNormal[ 2 ].z = vTransNormal[ 2 ].y;

		//vecSaveNormal[ 0 ].Normalize();
		//vecSaveNormal[ 1 ].Normalize();
		//vecSaveNormal[ 2 ].Normalize();

		// ��ȡ3������Ķ���ɫ
		if( nColor > 0 )
		{
			clrSaveColor[ 0 ] = clrColorList[ vecCFaceList[ cFace * 3 + 0 ] ];
			clrSaveColor[ 1 ] = clrColorList[ vecCFaceList[ cFace * 3 + 1 ] ];
			clrSaveColor[ 2 ] = clrColorList[ vecCFaceList[ cFace * 3 + 2 ] ];
		}
		else
		{
			clrSaveColor[ 0 ].c = 0xFFFFFFFF;
			clrSaveColor[ 1 ].c = 0xFFFFFFFF;
			clrSaveColor[ 2 ].c = 0xFFFFFFFF;
		}

		if ( !vecWeightList.empty() )
			MakeSaveVertex( vecSavePosition, vecSaveNormal, vecSaveTexture, clrSaveColor, weight, pMtl, vecMatList[ cFace ] );
		else
			MakeSaveVertex( vecSavePosition, vecSaveNormal, vecSaveTexture, clrSaveColor, NULL, pMtl, vecMatList[ cFace ] );
	}
	MakeR3DMesh( pMeshObject );

	m_lstMesh.push_back( pMeshObject );
	ClearSaveBuffers();
}

// ��ȡģ�͵�ID
int Red3DMaxEffectUtil2008::GetMeshID( INode *pNode )
{
	for( UINT cMesh = 0; cMesh < m_lstMesh.size(); cMesh++ )
	{
		if( m_lstMesh[ cMesh ]->m_pNode == pNode )
			return cMesh;
	}
	return -1;
}

/*!	\fn MakeR3DMesh
 *	\param pMesh 3DsMAXģ��ָ��
 *	\brief ����R3D���������ģ��
 */
void Red3DMaxEffectUtil2008::MakeR3DMesh( RMesh *pMesh )
{
	int nVertexOffset = 0;
	int nIndexOffset = 0;
	int nTotalVertex = 0;
	int nTotalIndex = 0;	

	// ����ͼԪ
	// һ��ģ�������50��ͼԪ
	for( int cPrimitive=0; cPrimitive < MAX_NODE; cPrimitive++ )
	{
		// �����ǰͼԪ�ı���λ��Ϊ�������
		if( m_lstSavePosition[ cPrimitive ].empty() )
			continue;

		pMesh->m_lstVertexCount.push_back( (int)m_lstSavePosition[ cPrimitive ].size() );			// ���������
		pMesh->m_lstIndexCount.push_back( (int)m_lstSaveIndex[ cPrimitive ].size() );				// ����������

		pMesh->m_lstVertexOffset.push_back( nTotalVertex );											// ��¼��ͼԪ�ڶ��㻺���е�ƫ��
		pMesh->m_lstIndexOffset.push_back( nTotalIndex );											// ��¼��ͼԪ�ڶ������������е�ƫ��

		pMesh->m_lstTextureID.push_back( FindVector( m_lstFinalTextureName, m_lstSaveTextureName[ cPrimitive ] ) );

		// ���Ӷ������������ܸ���
		nTotalVertex += (int)m_lstSavePosition[ cPrimitive ].size();
		nTotalIndex += (int)m_lstSaveIndex[ cPrimitive ].size();

		Point3 *pNormal = new Point3[ m_lstSavePosition[ cPrimitive ].size() ];						// ��ǰͼԪ���淨��

		// ����ÿ������
		for( UINT cVertex=0; cVertex < m_lstSavePosition[ cPrimitive ].size(); cVertex++ )
		{
			pMesh->m_lstPosition.push_back( m_lstSavePosition[ cPrimitive ][ cVertex ] );				// ѹ�붥���λ��
			pMesh->m_lstTextureCoord.push_back( m_lstSaveTexture[ cPrimitive ][ cVertex ] );			// ѹ�붥���UV����
			pMesh->m_lstColor.push_back( m_lstSaveColor[ cPrimitive ][ cVertex ] );						// ѹ�붥�����ɫ
			pNormal[ cVertex ] = m_lstSaveNormal[ cPrimitive ][ cVertex ];								// ѹ�붥��ķ��߷���
			// �������Ȩ�ر�Ϊ��,��ѹ�붥���Ȩ��
			if ( !m_lstSaveWeight[ cPrimitive ].empty() )
				pMesh->m_vecWeightList.push_back( m_lstSaveWeight[ cPrimitive ][ cVertex ] );
		}

		// �������ͼԪ�Ķ�������
		for( UINT cIndex=0; cIndex < m_lstSaveIndex[ cPrimitive ].size(); cIndex++ )
		{
			pMesh->m_lstIndex.push_back( m_lstSaveIndex[ cPrimitive ][ cIndex ] );
		}
		
		int nVertexIndex[ 3 ];
		Point3 vecPosition[ 3 ];
		Point2 vecTextureCoordinate[ 3 ];
		Point3 vecS[ 3 ], vecT[ 3 ];

		// ͼԪ�еĶ��㶼��Ӧһ����������UV
		Point3 *pTangentU = new Point3[ m_lstSavePosition[ cPrimitive ].size() ];
		Point3 *pTangentV = new Point3[ m_lstSavePosition[ cPrimitive ].size() ];		
		memset( pTangentU, 0, sizeof( Point3 ) * m_lstSavePosition[ cPrimitive ].size() );
		memset( pTangentV, 0, sizeof( Point3 ) * m_lstSavePosition[ cPrimitive ].size() );

		// ��������ͼԪ
		for( int cPolygon=0; cPolygon < (int)m_lstSaveIndex[ cPrimitive ].size() / 3; cPolygon++ )
		{
			// ��ȡͼԪ���������������
			nVertexIndex[ 0 ] = pMesh->m_lstIndex[ cPolygon * 3 + 0 ];
			nVertexIndex[ 1 ] = pMesh->m_lstIndex[ cPolygon * 3 + 1 ];
			nVertexIndex[ 2 ] = pMesh->m_lstIndex[ cPolygon * 3 + 2 ];

			// ���������λ��
			vecPosition[ 0 ] = pMesh->m_lstPosition[ nVertexIndex[ 0 ] ];
			vecPosition[ 1 ] = pMesh->m_lstPosition[ nVertexIndex[ 1 ] ];
			vecPosition[ 2 ] = pMesh->m_lstPosition[ nVertexIndex[ 2 ] ];

			// �����������������
			vecTextureCoordinate[ 0 ] = pMesh->m_lstTextureCoord[ nVertexIndex[ 0 ] ];
			vecTextureCoordinate[ 1 ] = pMesh->m_lstTextureCoord[ nVertexIndex[ 1 ] ];
			vecTextureCoordinate[ 2 ] = pMesh->m_lstTextureCoord[ nVertexIndex[ 2 ] ];

			// ��ʼ��UV��������
			vecS[ 0 ] = Point3( 0.0f, 0.0f, 0.0f );
			vecS[ 1 ] = Point3( 0.0f, 0.0f, 0.0f );
			vecS[ 2 ] = Point3( 0.0f, 0.0f, 0.0f );
			vecT[ 0 ] = Point3( 0.0f, 0.0f, 0.0f );
			vecT[ 1 ] = Point3( 0.0f, 0.0f, 0.0f );
			vecT[ 2 ] = Point3( 0.0f, 0.0f, 0.0f );

			// ����UV����
			ComputeTangentVector( vecPosition[ 0 ], vecPosition[ 1 ], vecPosition[ 2 ], 
								 vecTextureCoordinate[ 0 ], vecTextureCoordinate[ 1 ], vecTextureCoordinate[ 2 ],
								 vecT, vecS );

			// ���3���������������UV
			pTangentU[ nVertexIndex[ 0 ] ] += vecS[ 0 ];
			pTangentU[ nVertexIndex[ 1 ] ] += vecS[ 1 ];
			pTangentU[ nVertexIndex[ 2 ] ] += vecS[ 2 ];

			pTangentV[ nVertexIndex[ 0 ] ] += vecT[ 0 ];
			pTangentV[ nVertexIndex[ 1 ] ] += vecT[ 1 ];
			pTangentV[ nVertexIndex[ 2 ] ] += vecT[ 2 ];			
		}

		color clrNormal, clrTangentU, clrTangentV;

		// ͳһÿ����������������뷨������ͳһ�Ķ�����
		for( UINT cVertex=0; cVertex < m_lstSavePosition[ cPrimitive ].size(); cVertex++ )
		{			
			pNormal[ cVertex ].Normalize();
			pTangentU[ cVertex ].Normalize();
			pTangentV[ cVertex ].Normalize();

			clrNormal.b = ( unsigned char ) ( ( pNormal[ cVertex ].x + 1.0f ) * 0.5f * 255.0f );
			clrNormal.g = ( unsigned char ) ( ( pNormal[ cVertex ].y + 1.0f ) * 0.5f * 255.0f );
			clrNormal.r = ( unsigned char ) ( ( pNormal[ cVertex ].z + 1.0f ) * 0.5f * 255.0f );

			clrTangentU.b = ( unsigned char ) ( ( pTangentU[ cVertex ].x + 1.0f ) * 0.5f * 255.0f );
			clrTangentU.g = ( unsigned char ) ( ( pTangentU[ cVertex ].y + 1.0f ) * 0.5f * 255.0f );
			clrTangentU.r = ( unsigned char ) ( ( pTangentU[ cVertex ].z + 1.0f ) * 0.5f * 255.0f );

			clrTangentV.b = ( unsigned char ) ( ( pTangentV[ cVertex ].x + 1.0f ) * 0.5f * 255.0f );
			clrTangentV.g = ( unsigned char ) ( ( pTangentV[ cVertex ].y + 1.0f ) * 0.5f * 255.0f );
			clrTangentV.r = ( unsigned char ) ( ( pTangentV[ cVertex ].z + 1.0f ) * 0.5f * 255.0f );			

			pMesh->m_lstNormal.push_back( clrNormal );
			pMesh->m_lstTangentU.push_back( clrTangentU );
			pMesh->m_lstTangentV.push_back( clrTangentV );
		}
		
		delete [] pTangentU;
		delete [] pTangentV;
		delete [] pNormal;
	}	

	pMesh->m_nVertexCount = nTotalVertex;				// ���������
	pMesh->m_nIndexCount = nTotalIndex;					// ����������
}

/*!	\fn MakeSaveVertex	
 *	\param pPosition ͼԪ���������λ���б�
 *	\param pNormal ͼԪ����������ķ����б�
 *	\param pTexture ͼԪ����������������б�
 *	\param pColor ͼԪ��������Ķ���ɫ�б�
 *	\param pWeight ͼԪ����������Ȩ�ص�ָ��
 *	\param pMtl ͼԪ�Ĳ���ָ��
 *	\param nSubMat ͼԪ�Ĳ���ID
 *	\brief ���浽���㱣�滺��
 */
bool Red3DMaxEffectUtil2008::MakeSaveVertex( Point3 *pPosition, Point3 *pNormal, Point2 *pTexture, color *pColor, RWeight * pWeight, Mtl *pMtl, int nSubMat )
{
	// �ҵ���������
	int nMat = FindVector( m_lstMAXMatrial, pMtl );
	if( nMat == -1 )
		return false;

	int nTextureIndex = -1;

	// ���������б��ҵ���֮��ԵĲ��ʲ���¼��������

	// ��һ���ж�������ID��������ID��ͬ�����
	for( UINT cTexture=0; cTexture < m_lstTexture.size(); cTexture++ )
	{
		if( m_lstTexture[ cTexture ].m_nMatID == nMat &&
			m_lstTexture[ cTexture ].m_nMatSubID == nSubMat )
		{
			nTextureIndex = cTexture;
			break;
		}
	}

	// ���û���ҵ������ڶ��α���,��ν�Ѱ��������ID��ƥ�����
	if( nTextureIndex == -1 )
	{
		for( UINT cTexture=0; cTexture < m_lstTexture.size(); cTexture++ )
		{
			if( m_lstTexture[ cTexture ].m_nMatID == nMat )				
			{
				nTextureIndex = cTexture;
				break;
			}
		}
		if( nTextureIndex == -1 )
			return false;
	}

	// ͨ��nTextureIndex��m_lstTexture�л�ȡ������ļ���
	// 

    int AddVertexIndex = -1;			// ͼԪ����
	AddVertexIndex = FindVector( m_lstSaveTextureName, m_lstTexture[ nTextureIndex ].m_strTextureName );
	if( AddVertexIndex == -1 )
	{
		// ���û���ҵ�����������ͼ����
		m_lstSaveTextureName.push_back( m_lstTexture[ nTextureIndex ].m_strTextureName );
		AddVertexIndex = (int)m_lstSaveTextureName.size() - 1;
	}

	// ����ͼԪ����������
	for( int cVertex=0; cVertex < 3; cVertex++ )
	{
		bool bAlready = false;
		/*
		UINT cCheck;
		for( cCheck=0; cCheck < m_lstSavePosition[ AddVertexIndex ].size(); cCheck++ )
		{
			if( m_lstSavePosition[ AddVertexIndex ][ cCheck ] == pPosition[ cVertex ] &&
				m_lstSaveNormal[ AddVertexIndex ][ cCheck ] == pNormal[ cVertex ] &&
				m_lstSaveTexture[ AddVertexIndex ][ cCheck ] == pTexture[ cVertex ] )
			{
				bAlready = true;
				break;
			}
		}
 
		if( bAlready )
		{
			m_lstSaveIndex[ AddVertexIndex ].push_back( cCheck );
		}
		else*/
		{
			// AddVertexIndex�����ֵ��ͼԪ��ģ���е�����
			m_lstSavePosition[ AddVertexIndex ].push_back( pPosition[ cVertex ] );			// һ������һ��λ�ö���
			m_lstSaveNormal[ AddVertexIndex ].push_back( pNormal[ cVertex ] );				// һ������һ������
			m_lstSaveTexture[ AddVertexIndex ].push_back( pTexture[ cVertex ] );			// һ������һ������UV����
			m_lstSaveColor[ AddVertexIndex ].push_back( pColor[ cVertex ] );				// һ������һ������ֵ
			m_lstSaveIndex[ AddVertexIndex ].push_back( (WORD)m_lstSavePosition[ AddVertexIndex ].size() - 1 );	// ���������
			if ( pWeight )																	// �����Ȩֵ
				m_lstSaveWeight[ AddVertexIndex ].push_back( pWeight[ cVertex ] );
		}
	}	
	return true;
}

/*!	\fn ClearSaveBuffers
 *	\brief ������滺��
 */
void Red3DMaxEffectUtil2008::ClearSaveBuffers(void)
{
	// һ��ģ�����50��ͼԪ
	for( int cNode = 0; cNode < MAX_NODE; cNode++ )
	{
		// �����ͼԪ��λ�ýڵ�Ϊ�������
		if( m_lstSavePosition[ cNode ].empty() )
			continue;
		m_lstSavePosition[ cNode ].resize( 0 );
		m_lstSaveNormal [ cNode ].resize( 0 );
		m_lstSaveTexture[ cNode ].resize( 0 );
		m_lstSaveColor[ cNode ].resize( 0 );
		m_lstSaveIndex[ cNode ].resize( 0 );
		m_lstSaveWeight[ cNode ].resize( 0 );
	}
}

/*!	\fn ExportAnimationElement
 *	\brief �������Ԫ��
 *	\details ������������ģ��,���ұ���ÿ֡,��ÿ��ģ����ÿ֡�µ� "��ת" "ƽ��" "����" ����״̬���浽��ǰģ�͵�
 *			 ĳ������
 */
void Red3DMaxEffectUtil2008::ExportAnimationElement(void)
{
	RMesh *pMesh;

	// ��������ģ��
	for( UINT cMesh = 0; cMesh < m_lstMesh.size(); cMesh++ )
	{
		// ��ȡһ��ģ��
		pMesh = m_lstMesh[ cMesh ];
		TimeValue	start = ip->GetAnimRange().Start();
		TimeValue	end = ip->GetAnimRange().End();
		int			delta = GetTicksPerFrame();
		TimeValue	t;
		Matrix3		tm;
		AffineParts ap;
		Point3		pos, prevPos;
		int nFrame = 1;
		
		Quat qPrev;
		qPrev.Identity();

		Point3		scale;

		// ����ÿ֡
		for( t = start; t < end + delta; t += delta )
		{
			//Matrix3 mtTM = pMesh->m_pNode->GetNodeTM( t ) * Inverse( pMesh->m_pNode->GetParentTM( t ) );
			Matrix3 mtTM = pMesh->m_pNode->GetNodeTM( t );		// ��ȡ�ڵ���������
			
			RHelper::MAXtoD3D( mtTM );
			decomp_affine( mtTM, &ap );

			// �õ�λ��
			pos = ap.t;
			Point3 vecPosKey;

			vecPosKey.x = pos.x;
			vecPosKey.y = pos.y;
			vecPosKey.z = pos.z;

			pMesh->m_lstAnimationPositionKey.push_back( vecPosKey );			// λ����Ϣ

			//Quat q = ap.q / qPrev;
			Quat q = ap.q;
			//qPrev = q;
			
			Point4 vecRotateKey;
			vecRotateKey.x = -q.x;
			vecRotateKey.y = -q.y;
			vecRotateKey.z = -q.z;
			vecRotateKey.w = q.w;
			pMesh->m_lstAnimationRotateKey.push_back( vecRotateKey );			// ��ת��Ϣ

			scale = ap.k;
			Point3 vecScaleKey;
			vecScaleKey.x = scale.x;
			vecScaleKey.y = scale.y;
			vecScaleKey.z = scale.z;			

			pMesh->m_lstAnimationScaleKey.push_back( vecScaleKey );				// ������Ϣ

			float fVisibility = pMesh->m_pNode->GetVisibility( t );		
			pMesh->m_lstAnimationVisibilityKey.push_back( fVisibility );		// �ɼ���Χ��Ϣ

			nFrame++;			
		}
		//D3DXQUATERNION qtRotation;

		/*for( int cRotation=1; cRotation < pMesh->m_lstAnimationRotateKey.GetCount(); cRotation++ )
		{			
			memcpy( &qtRotation, &pMesh->m_lstAnimationRotateKey[ cRotation ], sizeof( D3DXQUATERNION ) );			 
			D3DXQuaternionMultiply( (D3DXQUATERNION*)&pMesh->m_lstAnimationRotateKey[ cRotation ], (D3DXQUATERNION*)&pMesh->m_lstAnimationRotateKey[ cRotation - 1], &qtRotation );			
		}*/
	}
}

/*

void Red3DMaxEffectUtil2008::ExportAnimationRotate(void)
{
	return;
	RMesh *pMesh;
	for( int cMesh = 0; cMesh < m_lstMesh.GetCount(); cMesh++ )
	{
		pMesh = m_lstMesh[ cMesh ];
		TimeValue	start = ip->GetAnimRange().Start();
		TimeValue	end = ip->GetAnimRange().End();
		int			delta = GetTicksPerFrame();
		TimeValue	t;
		Matrix3		tm;
		AffineParts ap;
		Quat		prevQ;
		prevQ.Identity();
		for( t = start; t < end + delta; t += delta )
		{
			tm = pMesh->m_pNode->GetNodeTM( t );
			RHelper::MAXtoD3D( tm );

			matrix mat;
			mat.MakeIdent();
			 
			mat._11 = tm.GetRow( 0 ).x;
			mat._12 = tm.GetRow( 0 ).y;
			mat._13 = tm.GetRow( 0 ).z;

			mat._21 = tm.GetRow( 1 ).x;
			mat._22 = tm.GetRow( 1 ).y;
			mat._23 = tm.GetRow( 1 ).z;

			mat._31 = tm.GetRow( 2 ).x;
			mat._32 = tm.GetRow( 2 ).y;
			mat._33 = tm.GetRow( 2 ).z;

			mat._41 = tm.GetRow( 3 ).x;
			mat._42 = tm.GetRow( 3 ).y;
			mat._43 = tm.GetRow( 3 ).z;

			vector3 vecScale;
			D3DXQUATERNION quat;
			vector3 vecTrans;
			D3DXMatrixDecompose( vecScale, &quat, vecTrans, mat );
			
			Quat qTemp;
			qTemp.x = quat.x;
			qTemp.y = quat.y;
			qTemp.z = quat.z;
			qTemp.w = quat.w;

			Quat q = qTemp / prevQ;
	
			prevQ = qTemp;
			vector4 vecRotateKey;
			vecRotateKey.x = -q.x;
			vecRotateKey.y = -q.y;
			vecRotateKey.z = -q.z;
			vecRotateKey.w = q.w;
			pMesh->m_lstAnimationRotateKey.Add( vecRotateKey );
		}

		D3DXQUATERNION qtRotation;

		for( int cRotation=1; cRotation < pMesh->m_lstAnimationRotateKey.GetCount(); cRotation++ )
		{			
			memcpy( &qtRotation, &pMesh->m_lstAnimationRotateKey[ cRotation ], sizeof( D3DXQUATERNION ) );			 
			D3DXQuaternionMultiply( (D3DXQUATERNION*)&pMesh->m_lstAnimationRotateKey[ cRotation ], (D3DXQUATERNION*)&pMesh->m_lstAnimationRotateKey[ cRotation - 1], &qtRotation );			
		}
	}
}

void Red3DMaxEffectUtil2008::ExportAnimationScale(void)
{
	RMesh *pMesh;
	for( int cMesh = 0; cMesh < m_lstMesh.GetCount(); cMesh++ )
	{
		pMesh = m_lstMesh[ cMesh ];
		TimeValue	start = ip->GetAnimRange().Start();
		TimeValue	end = ip->GetAnimRange().End();
		int			delta = GetTicksPerFrame();
		TimeValue	t;
		Matrix3		tm;
		AffineParts ap;
		Point3		scale, prevScale;
		int nFrame = 1;
		for( t = start; t < end + delta; t += delta )
		{
            tm = pMesh->m_pNode->GetNodeTM( t );
			RHelper::MAXtoD3D( tm );
			decomp_affine( tm, &ap );
			scale = ap.k;
			
			vector3 vecScaleKey;
			vecScaleKey.x = scale.x;
			vecScaleKey.y = scale.y;
			vecScaleKey.z = scale.z;			
			pMesh->m_lstAnimationScaleKey.Add( vecScaleKey );
			prevScale = scale;
			nFrame++;
		}
	}
}

void Red3DMaxEffectUtil2008::ExportAnimationVisibility(void)
{
	RMesh *pMesh;
	for( int cMesh = 0; cMesh < m_lstMesh.GetCount(); cMesh++ )
	{
		pMesh = m_lstMesh[ cMesh ];
		TimeValue	start = ip->GetAnimRange().Start();
		TimeValue	end = ip->GetAnimRange().End();
		int			delta = GetTicksPerFrame();
		TimeValue	t;
		Matrix3		tm;
		AffineParts ap;
		float		fVisibility;
		int nFrame = 1;
		for( t = start; t < end + delta; t += delta )
		{			
			nFrame++;
		}
	}
}
*/

/*!	\fn ExportAnimationMesh
 *	\param pNode ����ڵ�
 *	\param t ����ʱ��ڵ�
 *	\param iAniType ��������
 *	\brief �������ģ��
 */
void Red3DMaxEffectUtil2008::ExportAnimationMesh( INode *pNode, TimeValue t, int iAniType )
{
	Mtl *pMtl = pNode->GetMtl();
	if( pMtl == NULL )
		return;	

	// �ڵ�ĳ���
	int v1, v2, v3;	
	bool bNegScale = RHelper::TMNegParity( pNode->GetNodeTM( t ) );

	if( bNegScale )
	{
		v1 = 0; v2 = 1; v3 = 2;
	}
	else
	{
		v1 = 2; v2 = 1; v3 = 0;
	}

	// �ӽڵ��е�ǰʱ���ȡ����ͼԪ����
	bool bNeedDel;
	TriObject *pTri = RHelper::GetTriObjectFromNode( pNode, t, bNeedDel );
	if( pTri == NULL )	
		return;

	// ������ͼԪ�����л�ȡ����ģ��,��������
	Mesh *pMesh = &pTri->GetMesh();
	pMesh->buildNormals();

	// ��ȡ��ǰ�ڵ�ĸ��ڵ��볡�����ڵ�
	INode *pParent = pNode->GetParentNode();
	INode *pRoot = ip->GetRootNode();

	// ��ȡ��ǰ�ڵ��ģ��ID
	int nMesh = GetMeshID( pNode );
	if( nMesh == -1 ) 
		return;

	// ����һ�����񶯻��ڴ�
	RMeshAnimation *pMeshObject = new RMeshAnimation;				
	
	vector< Point3 > vecPositionList;			// λ�ö���,��¼��ÿ�������λ��
	vector< Point2 > vecTextureList;			// ��������
	vector< Point3 > vecNormalList;				// �����б�
	vector< color > clrColorList;				// ������ɫ�б�
	vector< DWORD > vecFaceList;				// ͼԪ�б�
	vector< DWORD > vecTFaceList;				//
	vector< WORD > vecMatList;					// �����б�,��������ͼԪΪ��λ
	vector< DWORD > vecCFaceList;				// 

	int nVertex	= pMesh->getNumVerts();			// ��ȡģ�Ͷ�������
	int nFace = pMesh->getNumFaces();			// ��ȡ��ǰģ������
	int nTexture = pMesh->getNumTVerts();		// ��ȡ��ǰ�������
	int nColor = pMesh->numCVerts;				// ��ȡ��ǰģ�Ͷ�����ɫ����

	Point3 p3Position, p3Normal, p3Texture, p3Color;
	Point3 vecPosition, vecNormal;
	Point2 vecTexture;		

	Matrix3 mtWorld = pNode->GetObjectTM( 0 );	// ��ȡ��ǰ�ڵ���������	
	Matrix3 mtInvTM = Inverse( mtWorld );		// ��ȡ��ǰ�ڵ�����������
	
	// ����һ���������ж��������֡ʱ��λ��
	for( int cVertex = 0; cVertex < nVertex; cVertex++ )
	{
		vecPosition = RHelper::MAXtoR3D( pMesh->verts[ cVertex ] );			// ��3dsmax����ת��Ϊr3d�ľ���
		vecPositionList.push_back( vecPosition );							// ѹ��λ�ö���
	}

	// ����һ���������ж��������֡ʱ��UVλ��
	for( int cTexture=0; cTexture < nTexture; cTexture++ )
	{
		p3Texture = pMesh->tVerts[ cTexture ];								// ȡ�����������ṹ
		// �������޶���[0 1]֮��
		vecTexture.x = p3Texture.x;
		vecTexture.y = 1.0f - p3Texture.y;
		vecTextureList.push_back( vecTexture );								// ѹ��һ�����㵱ǰ������
	}

	// ����һ���������ж��������֡ʱ��ɫ
	for( int cColor=0; cColor < nColor; cColor++ )
	{
		p3Color = pMesh->vertCol[ cColor ];									// ȡ���������ɫ
		color clr;
		clr.a = 0x0;
		clr.r = ( unsigned char )( p3Color.x * 255.0f );
		clr.g = ( unsigned char )( p3Color.y * 255.0f );
		clr.b = ( unsigned char )( p3Color.z * 255.0f );

		clrColorList.push_back( clr );										// ��0-1�ķ�Χ��չ��0-255��Χ��ѹ���б�
	}

	// ����ͼԪ����
	for( int cFace = 0; cFace < nFace; cFace++ )					
	{
		// ѹ���������������
		vecFaceList.push_back( pMesh->faces[ cFace ].v[ v1 ] );
		vecFaceList.push_back( pMesh->faces[ cFace ].v[ v2 ] );
		vecFaceList.push_back( pMesh->faces[ cFace ].v[ v3 ] );

		// ѹ�뵱ǰͼԪ�Ĳ���ID
		vecMatList.push_back( pMesh->faces[ cFace ].getMatID() );				

		vecTFaceList.push_back( pMesh->tvFace[ cFace ].t[ v1 ] );
		vecTFaceList.push_back( pMesh->tvFace[ cFace ].t[ v2 ] );
		vecTFaceList.push_back( pMesh->tvFace[ cFace ].t[ v3 ] );		

		// �����ɫ����0
		if( nColor > 0 )
		{		
			vecCFaceList.push_back( pMesh->vcFace[ cFace ].t[ v1 ] );
			vecCFaceList.push_back( pMesh->vcFace[ cFace ].t[ v2 ] );
			vecCFaceList.push_back( pMesh->vcFace[ cFace ].t[ v3 ] );
		}

	}

	Point3 vecSavePosition[ 3 ], vecSaveNormal[ 3 ], vecSaveTangentU[ 3 ], vecSaveTangentV[ 3 ];
	Point2 vecSaveTexture[ 3 ];
	color clrSaveColor[ 3 ];

	// ���ƽ������
	mtWorld.NoTrans();

	// ��������ͼԪ
	for( int cFace=0; cFace < nFace ; cFace++ )
	{
		// vecFaceList�б�����Ƕ�������
		// vecSavePosition�����˶������ά����
		vecSavePosition[ 0 ] = vecPositionList[ vecFaceList[ cFace * 3 + 0 ] ];
		vecSavePosition[ 1 ] = vecPositionList[ vecFaceList[ cFace * 3 + 1 ] ];
		vecSavePosition[ 2 ] = vecPositionList[ vecFaceList[ cFace * 3 + 2 ] ];

		// vecSavePosition�����˶������������
		vecSaveTexture[ 0 ] = vecTextureList[ vecTFaceList[ cFace * 3 + 0 ] ];
		vecSaveTexture[ 1 ] = vecTextureList[ vecTFaceList[ cFace * 3 + 1 ] ];
		vecSaveTexture[ 2 ] = vecTextureList[ vecTFaceList[ cFace * 3 + 2 ] ];

		// ȡ����ǰͼԪ
		Face* f;
		f = &pMesh->faces[ cFace ];

		// ��ȡ������������������㷨��
		vecSaveNormal[ 0 ] = RHelper::GetVertexNormal( pMesh, cFace, pMesh->getRVertPtr( f->getVert( v1 ) ) );
		vecSaveNormal[ 1 ] = RHelper::GetVertexNormal( pMesh, cFace, pMesh->getRVertPtr( f->getVert( v2 ) ) );
		vecSaveNormal[ 2 ] = RHelper::GetVertexNormal( pMesh, cFace, pMesh->getRVertPtr( f->getVert( v3 ) ) );

		// ת����ʽ��r3d�����������±��浽vecSaveNormal
		Point3 vNormal[ 3 ];

		vNormal[ 0 ].x = vecSaveNormal[ 0 ].x;
		vNormal[ 0 ].y = vecSaveNormal[ 0 ].y;
		vNormal[ 0 ].z = vecSaveNormal[ 0 ].z;

		vNormal[ 1 ].x = vecSaveNormal[ 1 ].x;
		vNormal[ 1 ].y = vecSaveNormal[ 1 ].y;
		vNormal[ 1 ].z = vecSaveNormal[ 1 ].z;

		vNormal[ 2 ].x = vecSaveNormal[ 2 ].x;
		vNormal[ 2 ].y = vecSaveNormal[ 2 ].y;
		vNormal[ 2 ].z = vecSaveNormal[ 2 ].z;

		//Point3 vTransNormal[ 3 ];

		vecSaveNormal[ 0 ] = RHelper::MAXtoR3D( vNormal[ 0 ] );
		vecSaveNormal[ 1 ] = RHelper::MAXtoR3D( vNormal[ 1 ] );
		vecSaveNormal[ 2 ] = RHelper::MAXtoR3D( vNormal[ 2 ] );

		//vecSaveNormal[ 0 ].x = vTransNormal[ 0 ].x;
		//vecSaveNormal[ 0 ].y = vTransNormal[ 0 ].z;
		//vecSaveNormal[ 0 ].z = vTransNormal[ 0 ].y;

		//vecSaveNormal[ 1 ].x = vTransNormal[ 1 ].x;
		//vecSaveNormal[ 1 ].y = vTransNormal[ 1 ].z;
		//vecSaveNormal[ 1 ].z = vTransNormal[ 1 ].y;

		//vecSaveNormal[ 2 ].x = vTransNormal[ 2 ].x;
		//vecSaveNormal[ 2 ].y = vTransNormal[ 2 ].z;
		//vecSaveNormal[ 2 ].z = vTransNormal[ 2 ].y;

		//vecSaveNormal[ 0 ].Normalize();
		//vecSaveNormal[ 1 ].Normalize();
		//vecSaveNormal[ 2 ].Normalize();

		// ���������ɫ����0���¼��������Ӧ����Ķ�����ɫ
		if( nColor > 0 )
		{		
			clrSaveColor[ 0 ] = clrColorList[ vecCFaceList[ cFace * 3 + 0 ] ];
			clrSaveColor[ 1 ] = clrColorList[ vecCFaceList[ cFace * 3 + 1 ] ];
			clrSaveColor[ 2 ] = clrColorList[ vecCFaceList[ cFace * 3 + 2 ] ];
		}
		else
		{
			clrSaveColor[ 0 ].c = 0xFFFFFFFF;
			clrSaveColor[ 1 ].c = 0xFFFFFFFF;
			clrSaveColor[ 2 ].c = 0xFFFFFFFF;
		}

		// vecSavePosition			ͼԪ���������λ���б�
		// vecSaveNormal			ͼԪ����������ķ����б�
		// vecSaveTexture			ͼԪ����������������б�
		// clrSaveColor				ͼԪ��������Ķ���ɫ�б�
		// NULL						ͼԪ����������Ȩ�ص�ָ��
		// pMtl						ͼԪ�Ĳ���ָ��
		// vecMatList[ cFace ]		ͼԪ�Ĳ���ID
		// �������涥��
		MakeSaveVertex( vecSavePosition, vecSaveNormal, vecSaveTexture, clrSaveColor, NULL, pMtl, vecMatList[ cFace ] );
	}

	// ����r3d���񶯻�
	MakeR3DMeshAnimation( pMeshObject );

	// ѹ�뵱ǰ֡ģ�Ͷ����ṹ
	m_lstMesh[ nMesh ]->m_lstMeshAnimation.push_back( pMeshObject );
	ClearSaveBuffers();
}

void Red3DMaxEffectUtil2008::MakeR3DMeshAnimation( RMeshAnimation *pMesh )
{
	int nVertexOffset = 0;
	int nIndexOffset = 0;
	int nTotalVertex = 0;
	int nTotalIndex = 0;

	for( int cPrimitive=0; cPrimitive < MAX_NODE; cPrimitive++ )
	{
		if( m_lstSavePosition[ cPrimitive ].empty() )
			continue;

		Point3 *pNormal = new Point3[ m_lstSavePosition[ cPrimitive ].size() ];

		for( UINT cVertex=0; cVertex < m_lstSavePosition[ cPrimitive ].size(); cVertex++ )
		{
			pMesh->m_lstPosition.push_back( m_lstSavePosition[ cPrimitive ][ cVertex ] );			
			pMesh->m_lstTextureCoord.push_back( m_lstSaveTexture[ cPrimitive ][ cVertex ] );
			pMesh->m_lstColor.push_back( m_lstSaveColor[ cPrimitive ][ cVertex ] );

			pNormal[ cVertex ] = m_lstSaveNormal[ cPrimitive ][ cVertex ];
		}		
		
		int nVertexIndex[ 3 ];
		Point3 vecPosition[ 3 ];
		Point2 vecTextureCoordinate[ 3 ];
		Point3 vecS[ 3 ], vecT[ 3 ];

		Point3 *pTangentU = new Point3[ m_lstSavePosition[ cPrimitive ].size() ];
		Point3 *pTangentV = new Point3[ m_lstSavePosition[ cPrimitive ].size() ];		
		memset( pTangentU, 0, sizeof( Point3 ) * m_lstSavePosition[ cPrimitive ].size() );
		memset( pTangentV, 0, sizeof( Point3 ) * m_lstSavePosition[ cPrimitive ].size() );

		for( int cPolygon=0; cPolygon < (int)m_lstSaveIndex[ cPrimitive ].size() / 3; cPolygon++ )
		{
			nVertexIndex[ 0 ] = m_lstSaveIndex[ cPrimitive ][ cPolygon * 3 + 0 ];
			nVertexIndex[ 1 ] = m_lstSaveIndex[ cPrimitive ][ cPolygon * 3 + 1 ];
			nVertexIndex[ 2 ] = m_lstSaveIndex[ cPrimitive ][ cPolygon * 3 + 2 ];

			vecPosition[ 0 ] = pMesh->m_lstPosition[ nVertexIndex[ 0 ] ];
			vecPosition[ 1 ] = pMesh->m_lstPosition[ nVertexIndex[ 1 ] ];
			vecPosition[ 2 ] = pMesh->m_lstPosition[ nVertexIndex[ 2 ] ];

			vecTextureCoordinate[ 0 ] = pMesh->m_lstTextureCoord[ nVertexIndex[ 0 ] ];
			vecTextureCoordinate[ 1 ] = pMesh->m_lstTextureCoord[ nVertexIndex[ 1 ] ];
			vecTextureCoordinate[ 2 ] = pMesh->m_lstTextureCoord[ nVertexIndex[ 2 ] ];

			vecS[ 0 ] = Point3( 0.0f, 0.0f, 0.0f );
			vecS[ 1 ] = Point3( 0.0f, 0.0f, 0.0f );
			vecS[ 2 ] = Point3( 0.0f, 0.0f, 0.0f );
			vecT[ 0 ] = Point3( 0.0f, 0.0f, 0.0f );
			vecT[ 1 ] = Point3( 0.0f, 0.0f, 0.0f );
			vecT[ 2 ] = Point3( 0.0f, 0.0f, 0.0f );

			ComputeTangentVector( vecPosition[ 0 ], vecPosition[ 1 ], vecPosition[ 2 ], 
								 vecTextureCoordinate[ 0 ], vecTextureCoordinate[ 1 ], vecTextureCoordinate[ 2 ],
								 vecT, vecS );

			pTangentU[ nVertexIndex[ 0 ] ] += vecS[ 0 ];
			pTangentU[ nVertexIndex[ 1 ] ] += vecS[ 1 ];
			pTangentU[ nVertexIndex[ 2 ] ] += vecS[ 2 ];

			pTangentV[ nVertexIndex[ 0 ] ] += vecT[ 0 ];
			pTangentV[ nVertexIndex[ 1 ] ] += vecT[ 1 ];
			pTangentV[ nVertexIndex[ 2 ] ] += vecT[ 2 ];			
		}

		color clrNormal, clrTangentU, clrTangentV;

		for( UINT cVertex=0; cVertex < m_lstSavePosition[ cPrimitive ].size(); cVertex++ )
		{			
			pNormal[ cVertex ].Normalize();
			pTangentU[ cVertex ].Normalize();
			pTangentV[ cVertex ].Normalize();

			// ���㶥�㷨��
			clrNormal.b = ( unsigned char ) ( ( pNormal[ cVertex ].x + 1.0f ) * 0.5f * 255.0f );
			clrNormal.g = ( unsigned char ) ( ( pNormal[ cVertex ].y + 1.0f ) * 0.5f * 255.0f );
			clrNormal.r = ( unsigned char ) ( ( pNormal[ cVertex ].z + 1.0f ) * 0.5f * 255.0f );

			clrTangentU.b = ( unsigned char ) ( ( pTangentU[ cVertex ].x + 1.0f ) * 0.5f * 255.0f );
			clrTangentU.g = ( unsigned char ) ( ( pTangentU[ cVertex ].y + 1.0f ) * 0.5f * 255.0f );
			clrTangentU.r = ( unsigned char ) ( ( pTangentU[ cVertex ].z + 1.0f ) * 0.5f * 255.0f );

			clrTangentV.b = ( unsigned char ) ( ( pTangentV[ cVertex ].x + 1.0f ) * 0.5f * 255.0f );
			clrTangentV.g = ( unsigned char ) ( ( pTangentV[ cVertex ].y + 1.0f ) * 0.5f * 255.0f );
			clrTangentV.r = ( unsigned char ) ( ( pTangentV[ cVertex ].z + 1.0f ) * 0.5f * 255.0f );			

			pMesh->m_lstNormal.push_back( clrNormal );				// ѹ�붥�㷨��
			pMesh->m_lstTangentU.push_back( clrTangentU );			// ѹ������U
			pMesh->m_lstTangentV.push_back( clrTangentV );			// ѹ������V
		}

		nTotalVertex += (int)m_lstSavePosition[ cPrimitive ].size();
		nTotalIndex += (int)m_lstSaveIndex[ cPrimitive ].size();
		
		delete [] pTangentU;
		delete [] pTangentV;		
		delete [] pNormal;
	}	
}

// ���涯��ģ��
void Red3DMaxEffectUtil2008::MakeAnimationList(void)
{
	RMesh *pMesh;

	// ����ģ���б�
	for( UINT cMesh = 0; cMesh < m_lstMesh.size(); cMesh++ )
	{		
		pMesh = m_lstMesh[ cMesh ];

		// �����ģ�Ͷ���
		if ( pMesh->m_nAnimationType & REAT_MESH || pMesh->m_nAnimationType & REAT_TEXTURE )
		{
			TimeValue	start = ip->GetAnimRange().Start();			// ������ʼ
			TimeValue	end = ip->GetAnimRange().End();				// ��������
			int			delta = GetTicksPerFrame();					// һ֡���ٸ�Ticks
			TimeValue	t;
			// ����ÿ֡
			for( t = start; t < end + delta; t += delta )
			{
				// �������ģ��
				ExportAnimationMesh( pMesh->m_pNode, t, pMesh->m_nAnimationType );
			}
		}
	}

	// �������Ԫ��
	ExportAnimationElement();
	//ExportAnimationRotate();
	//ExportAnimationScale();
	//ExportAnimationVisibility();
}

// ����ͼԪ
bool Red3DMaxEffectUtil2008::MakeGeometry(void)
{
	// ���Ƚ�"e_"��ͷ��NODEҲ����������Ч���ӵĽڵ㶼��䵽lstExportList��
	vector< INode* > lstExportList;										// ���INode�б�
	FillNodeList( ip->GetRootNode(), lstExportList );					// ���ڵ��б�

	// �������б�Ϊ���򷵻�
	if( lstExportList.empty() ) 
		return false;

	// �����ڵ��б���������ڵ�
	for( UINT cNode = 0; cNode < lstExportList.size(); cNode++ )
	{
		ExportBone( lstExportList[ cNode ] );
	}

	// �����ڵ��б��������ڵ�
	for( UINT cNode = 0; cNode < lstExportList.size(); cNode++ )
	{
		ExportMesh( lstExportList[ cNode ] );
	}

	return true;
}

/*! \fn FillNodeList
 *	\param pNode ͼԪ�ڵ�
 *	\param lstNodeList ����ڵ��б�
 *	\brief ���ڵ��б�
 */
void Red3DMaxEffectUtil2008::FillNodeList( INode *pNode, vector< INode* > &lstNodeList )
{
	// ͼԪ�ڵ�����Ʊ�����"e_"��ͷ�����ǹ����ڵ�
	if( strncmp( pNode->GetName(), "e_", 2 ) == 0 || IsBone( pNode ) )
		lstNodeList.push_back( pNode );
	for( int cChild = 0; cChild < pNode->NumberOfChildren(); cChild++ )
		FillNodeList( pNode->GetChildNode( cChild ), lstNodeList );
}

// д����Ч������Ϣ��XML��
void Red3DMaxEffectUtil2008::r3dWriteEffectTextureToXML( FILE *fp, vector< string > &lstTexture ) {
	// д��������Ϣ
	int nTextureCount = (int)lstTexture.size();			// ��ȡ��������
	if (nTextureCount > 0) {
		fprintf(fp, "<Texture Size=\"%d\">\r\n", nTextureCount);
		for (int cTexture = 0; cTexture < nTextureCount; cTexture++) {
			fprintf(fp, "<TextureFileName>%s</TextureFilename>\r\n", lstTexture[ cTexture ].c_str());
		}
		fprintf(fp, "</Texture>\r\n");
	}
}

void Red3DMaxEffectUtil2008::r3dWriteFloatMatrixToXML( FILE *fp, float *fMatrix ) {
	fprintf(fp, "<R0>%f %f %f %f</R0>\r\n", fMatrix[0], fMatrix[1], fMatrix[2], fMatrix[3]);
	fprintf(fp, "<R1>%f %f %f %f</R1>\r\n", fMatrix[4], fMatrix[5], fMatrix[6], fMatrix[7]);
	fprintf(fp, "<R2>%f %f %f %f</R2>\r\n", fMatrix[8], fMatrix[9], fMatrix[10], fMatrix[11]);
	fprintf(fp, "<R3>%f %f %f %f</R3>\r\n", fMatrix[12], fMatrix[13], fMatrix[14], fMatrix[15]);
}

// д��ģ����Ϣ��XML��
bool Red3DMaxEffectUtil2008::r3dWriteEffectMeshToXML( FILE *fp, vector< Red3DMaxEffectUtil2008::RMesh* > &lstMesh ) {
	bool bBoneType = false;
	int nMeshCount = (int)lstMesh.size();												// ģ������
	if (nMeshCount <= 0) {
		return false;
	}

	fprintf(fp, "<Mesh Size=\"%d\">\r\n", nMeshCount);
	for (int cMesh = 0; cMesh < nMeshCount; cMesh++) {
		RMesh *pMesh = lstMesh[ cMesh ];												// ��ȡ����ģ��
		fprintf(fp, "<MeshNode>");
		fprintf(fp, "<FileName>%s</FileName>", pMesh->m_szName);
		fprintf(fp, "<NodeID>%d</NodeID>", pMesh->m_nNode);
		fprintf(fp, "<ParentID>%d</ParentID>", pMesh->m_nParent);

		// д��ģ�Ͷ����������󣬱��ؾ����Լ����������
		float fSave[ 16 ];
		RHelper::Matrix3ToFloat16( pMesh->m_matWorld, fSave );
		fprintf(fp, "<WorldMatrix>\r\n");
		r3dWriteFloatMatrixToXML(fp, fSave);
		fprintf(fp, "</WorldMatrix>\r\n");

		RHelper::Matrix3ToFloat16( pMesh->m_matLocal, fSave );
		fprintf(fp, "<LocalMatrix>\r\n");
		r3dWriteFloatMatrixToXML(fp, fSave);
		fprintf(fp, "</LocalMatrix>\r\n");

		RHelper::Matrix3ToFloat16( pMesh->m_matInvWorld, fSave );
		fprintf(fp, "<InvWorldMatrix>\r\n");
		r3dWriteFloatMatrixToXML(fp, fSave);
		fprintf(fp, "</InvWorldMatrix>\r\n");

		// д�붥���������������ݣ���������ͣ���������
		fprintf(fp, "<VertexCount>%d</VertexCount>\r\n", pMesh->m_nVertexCount);
		fprintf(fp, "<IndexCount>%d</IndexCount>\r\n", pMesh->m_nIndexCount);
		fprintf(fp, "<BillboardType>%d</BillboardType>\r\n", pMesh->m_nBillboardType);
		fprintf(fp, "<AnimationType>%d</AnimationType>\r\n", pMesh->m_nAnimationType);

		// ȷ������������
		if( pMesh->m_nAnimationType & REAT_MESH || pMesh->m_nAnimationType & REAT_TEXTURE )
			pMesh->m_bMeshAnimation = true;
		else
			pMesh->m_bMeshAnimation = false;

		fprintf(fp, "<IsMeshAnimation>%d</IsMeshAnimation>\r\n", pMesh->m_bMeshAnimation);

		int nTrack = (int)pMesh->m_lstAnimationPositionKey.size();								// �ؼ�֡֡������
		if( nTrack > 0 ) {
			fprintf(fp, "<AnimationPositionKey>%d</AnimationPositionKey>\r\n", nTrack);				// д��ؼ�֡����
		}

		// д��ͼԪ��Ϣ
		int nPrimitive = (int)pMesh->m_lstTextureID.size();										// ÿ��ͼԪ��Ӧһ������
		if (nPrimitive > 0) {
			fprintf(fp, "<PrimitiveCount>%d</PrimitiveCount>\r\n", nPrimitive);

			// д������ID,һ��ͼԪ��Ӧһ������
			fprintf(fp, "<TextureIDList Size=\"%d\">\r\n", nPrimitive);
			for (int cTextureID = 0; cTextureID < nPrimitive; cTextureID++) {
				fprintf(fp, "<ID>%d</ID>\r\n", pMesh->m_lstTextureID[ cTextureID ]);
			}
			fprintf(fp, "</TextureIDList>\r\n");
		}

		// д��λ������
		if (pMesh->m_lstPosition.size() > 0) {
			fprintf(fp, "<PositionList Size=\"%d\">\r\n", pMesh->m_lstPosition.size());
			for (int cPosition = 0; cPosition < pMesh->m_lstPosition.size(); cPosition++) {
				fprintf(fp, "<ID>%f %f %f</ID>\r\n", pMesh->m_lstPosition[ cPosition ].x, pMesh->m_lstPosition[ cPosition ].y, pMesh->m_lstPosition[ cPosition ].z);
			}
			fprintf(fp, "</PositionList>\r\n");
		}

		// д��UV����
		if (pMesh->m_lstTextureCoord.size() > 0) {
			fprintf(fp, "<UVList Size=\"%d\">\r\n", pMesh->m_lstTextureCoord.size());
			for (int cUV = 0; cUV < pMesh->m_lstTextureCoord.size(); cUV++) {
				fprintf(fp, "<ID>%f %f %f</ID>\r\n", pMesh->m_lstTextureCoord[ cUV ].x, pMesh->m_lstPosition[ cUV ].y);
			}
			fprintf(fp, "</UVList>\r\n");
		}

		// д�붥����ɫ�б�
		if (pMesh->m_lstColor.size() > 0) {
			fprintf(fp, "<ColorList Size=\"%d\">\r\n", pMesh->m_lstColor.size());
			for (int cColor = 0; cColor < pMesh->m_lstColor.size(); cColor++) {
				fprintf(fp, "<Color>%c %c %c %c</Color>\r\n", pMesh->m_lstColor[ cColor ].r, pMesh->m_lstColor[ cColor ].g, pMesh->m_lstColor[ cColor ].b, pMesh->m_lstColor[ cColor ].a);
			}
			fprintf(fp, "</ColorList>\r\n");
		}

		// д��ͼԪ����ķ�����ɫ�б�
		if (pMesh->m_lstNormal.size() > 0) {
			fprintf(fp, "<NormalList Size=\"%d\">\r\n", pMesh->m_lstNormal.size());
			for (int cNormal = 0; cNormal < pMesh->m_lstNormal.size(); cNormal++) {
				fprintf(fp, "<Normal>%c %c %c %c</Normal>\r\n", pMesh->m_lstNormal[ cNormal ].r, pMesh->m_lstNormal[ cNormal ].g, pMesh->m_lstNormal[ cNormal ].b, pMesh->m_lstNormal[ cNormal ].a);
			}
			fprintf(fp, "</NormalList>\r\n");
		}

		// д������U������
		if (pMesh->m_lstTangentU.size() > 0) {
			fprintf(fp, "<TangentUList Size=\"%d\">\r\n", pMesh->m_lstTangentU.size());
			for (int cTangentU = 0; cTangentU < pMesh->m_lstTangentU.size(); cTangentU++) {
				fprintf(fp, "<TangentU>%c %c %c %c</TangentU>\r\n", pMesh->m_lstTangentU[ cTangentU ].r, pMesh->m_lstTangentU[ cTangentU ].g, pMesh->m_lstTangentU[ cTangentU ].b, pMesh->m_lstTangentU[ cTangentU ].a);
			}
			fprintf(fp, "</TangentUList>\r\n");
		}

		// д������V������
		if (pMesh->m_lstTangentV.size() > 0) {
			fprintf(fp, "<TangentVList Size=\"%d\">\r\n", pMesh->m_lstTangentV.size());
			for (int cTangentV = 0; cTangentV < pMesh->m_lstTangentV.size(); cTangentV++) {
				fprintf(fp, "<TangentV>%c %c %c %c</TangentV>\r\n", pMesh->m_lstTangentV[ cTangentV ].r, pMesh->m_lstTangentV[ cTangentV ].g, pMesh->m_lstTangentV[ cTangentV ].b, pMesh->m_lstTangentV[ cTangentV ].a);
			}
			fprintf(fp, "</TangentVList>\r\n");
		}

		// д��ͼԪ����
		if (pMesh->m_lstIndex.size() > 0) {
			fprintf(fp, "<IndexList Size=\"%d\">\r\n", pMesh->m_lstIndex.size());
			for (int cIndex = 0; cIndex < pMesh->m_lstIndex.size(); cIndex++) {
				fprintf(fp, "<Index>%d</Index>\r\n", pMesh->m_lstIndex[ cIndex ]);
			}
			fprintf(fp, "</IndexList>\r\n");
		}

		// ��������
		if ( pMesh->m_nAnimationType & REAT_BONE )
		{
			vector< DWORD > vecBoneIndex;
			vector< DWORD > vecBoneWeight;
			vector< int > vecBoneTable;

			bBoneType = true;

			// ��ȡȨ���е�����
			int iWeightSize = (int) pMesh->m_vecWeightList.size();
			if (iWeightSize > 0) {
				// ����Ȩ���б�
				for ( int iWeight = 0; iWeight < iWeightSize; ++iWeight )
				{
					unsigned char nIndex[ 4 ];
					unsigned char nWeight[ 4 ];

					// ��ȡһ��Ȩ��ֵ
					RWeight & kWeight = pMesh->m_vecWeightList[ iWeight ];

					// һ��ģ�Ͷ�����4��������Ӱ��
					int iBone = 0;
					for (; iBone < 4; ++iBone )
					{
						// ��������Ϸ�ֵ,kWeight.vecBoneIndex.size()������4
						if ( iBone < (int) kWeight.vecBoneIndex.size() )
						{
							bool bFind = false;
							int iTable = 0;
							// ����������
							for ( ; iTable < (int) vecBoneTable.size(); ++iTable )
							{
								if ( vecBoneTable[ iTable ] == kWeight.vecBoneIndex[ iBone ]  )
								{
									bFind = true;
									break;
								}
							}

							// û���ҵ�����뵽��������
							if ( bFind == false )
							{
								vecBoneTable.push_back( kWeight.vecBoneIndex[ iBone ] );
								iTable = (int)vecBoneTable.size() - 1;
							}

							// ���ӹ���������Ȩ��
							nIndex[ iBone ] = iTable;
							nWeight[ iBone ] = (unsigned char)(kWeight.vecWeight[ iBone ] * 255.0f);
						}
						else
						{
							nIndex[ iBone ] = 0;
							nWeight[ iBone ] = 0;
						}
					}

					// ѹ�����
					vecBoneIndex.push_back( *((DWORD *) nIndex) );
					vecBoneWeight.push_back( *((DWORD *) nWeight) );
				}/* end for */
			}

			// д�������
			int iBoneTableSize = (int) vecBoneTable.size();
			if ( iBoneTableSize > 0 ) {
				fprintf(fp, "<BoneAnimation>\r\n");
					// ������
					fprintf(fp, "<BoneTable Size=\"%d\">\r\n", iBoneTableSize);
					for (int cBoneTable = 0; cBoneTable < iBoneTableSize; cBoneTable++) {
						fprintf(fp, "<Table>%d</Table>\r\n", vecBoneTable[ cBoneTable ]);
					}
					fprintf(fp, "</BoneTable>\r\n");

					// ��������
					fprintf(fp, "<BoneIndex Size=\"%d\">\r\n", iWeightSize);
					for (int cBoneIndex = 0; cBoneIndex < iWeightSize; cBoneIndex++) {
						fprintf(fp, "<Index>%d</Index>\r\n", vecBoneIndex[ cBoneIndex ]);
					}
					fprintf(fp, "</BoneIndex>\r\n");

					// ����Ȩ��
					fprintf(fp, "<BoneWeight Size=\"%d\">\r\n", iWeightSize);
					for (int cBoneWeight = 0; cBoneWeight < iWeightSize; cBoneWeight++) {
						fprintf(fp, "<Weight>%d</Weight>\r\n", vecBoneWeight[ cBoneWeight ]);
					}
					fprintf(fp, "</BoneWeight>\r\n");
				fprintf(fp, "</BoneAnimation>\r\n");
			}
		}
		// �ؼ�֡����
		else if( pMesh->m_nAnimationType & REAT_MESH || pMesh->m_nAnimationType & REAT_TEXTURE ) {
			int nAnimationMeshTrack = (int)pMesh->m_lstMeshAnimation.size();
			if (nAnimationMeshTrack > 0) {
				fprintf(fp, "<MeshOrTextureAnimation Size=\"%d\">\r\n", nAnimationMeshTrack);

				// ȡ��ÿ֡
				RMeshAnimation* pMeshAnimation;
				for( int cTrack=0; cTrack < nAnimationMeshTrack; cTrack++ )
				{
					pMeshAnimation = pMesh->m_lstMeshAnimation[ cTrack ];

					// ÿ������λ��
					int iAniSize = (int) pMeshAnimation->m_lstPosition.size();
					if ( iAniSize == 0 )
					{
						pMeshAnimation->m_lstPosition.resize( pMesh->m_nVertexCount );
						pMeshAnimation->m_lstColor.resize( pMesh->m_nVertexCount );
						pMeshAnimation->m_lstNormal.resize( pMesh->m_nVertexCount );
						pMeshAnimation->m_lstTangentU.resize( pMesh->m_nVertexCount );
						pMeshAnimation->m_lstTangentV.resize( pMesh->m_nVertexCount );
						pMeshAnimation->m_lstTextureCoord.resize( pMesh->m_nVertexCount );
					}
					// �����任λ���붥����������ͬ
					else if ( iAniSize < pMesh->m_nVertexCount )
					{
						// ��������任λ��С�ڶ�����������һЩ����λ��
						int iCount = pMesh->m_nVertexCount - iAniSize;
						for ( int iAdd = 0; iAdd < iCount; ++iAdd  )
						{
							pMeshAnimation->m_lstPosition.push_back( Point3(0,0,0) );
							pMeshAnimation->m_lstColor.push_back( color(0,0,0,0) );
							pMeshAnimation->m_lstNormal.push_back( color(0,0,0,0) );
							pMeshAnimation->m_lstTangentU.push_back( color(0,0,0,0) );
							pMeshAnimation->m_lstTangentV.push_back( color(0,0,0,0) );
							pMeshAnimation->m_lstTextureCoord.push_back( Point2(0.0f,0.0f) );
						}
					}

					// д��ÿ�������λ��
					fprintf(fp, "<PositionList Size=\"%d\">\r\n", pMeshAnimation->m_lstPosition.size());
					for (int cPosition = 0; cPosition < pMeshAnimation->m_lstPosition.size(); cPosition++) {
						fprintf(fp, "<Position>%f %f %f</Position>\r\n", pMeshAnimation->m_lstPosition[cPosition].x, pMeshAnimation->m_lstPosition[cPosition].y, pMeshAnimation->m_lstPosition[cPosition].z);
					}
					fprintf(fp, "</PositionList>\r\n");

					// д�붥����ɫ
					fprintf(fp, "<ColorList Size=\"%d\">\r\n", pMeshAnimation->m_lstColor.size());
					for (int cColor = 0; cColor < pMeshAnimation->m_lstColor.size(); cColor++) {
						fprintf(fp, "<Color>%c %c %c %c</Color>\r\n", pMeshAnimation->m_lstColor[cColor].r, pMeshAnimation->m_lstColor[cColor].g, pMeshAnimation->m_lstColor[cColor].b, pMeshAnimation->m_lstColor[cColor].a);
					}
					fprintf(fp, "</ColorList>\r\n");

					// д�붥�㷨��
					fprintf(fp, "<NormalList Size=\"%d\">\r\n", pMeshAnimation->m_lstNormal.size());
					for (int cNormal = 0; cNormal < pMeshAnimation->m_lstNormal.size(); cNormal++) {
						fprintf(fp, "<Normal>%c %c %c %c</Normal>\r\n", pMeshAnimation->m_lstNormal[cNormal].r, pMeshAnimation->m_lstNormal[cNormal].g, pMeshAnimation->m_lstNormal[cNormal].b, pMeshAnimation->m_lstNormal[cNormal].a);
					}
					fprintf(fp, "</NormalList>\r\n");

					// д������U
					fprintf(fp, "<TangentUList Size=\"%d\">\r\n", pMeshAnimation->m_lstTangentU.size());
					for (int cTangentU = 0; cTangentU < pMeshAnimation->m_lstTangentU.size(); cTangentU++) {
						fprintf(fp, "<TangentU>%c %c %c %c</TangentU>\r\n", pMeshAnimation->m_lstTangentU[cTangentU].r, pMeshAnimation->m_lstTangentU[cTangentU].g, pMeshAnimation->m_lstTangentU[cTangentU].b, pMeshAnimation->m_lstTangentU[cTangentU].a);
					}
					fprintf(fp, "</TangentUList>\r\n");

					// д������V
					fprintf(fp, "<TangentVList Size=\"%d\">\r\n", pMeshAnimation->m_lstTangentV.size());
					for (int cTangentV = 0; cTangentV < pMeshAnimation->m_lstTangentV.size(); cTangentV++) {
						fprintf(fp, "<TangentV>%c %c %c %c</TangentV>\r\n", pMeshAnimation->m_lstTangentV[cTangentV].r, pMeshAnimation->m_lstTangentV[cTangentV].g, pMeshAnimation->m_lstTangentV[cTangentV].b, pMeshAnimation->m_lstTangentV[cTangentV].a);
					}
					fprintf(fp, "</TangentVList>\r\n");

					// д��UV����
					fprintf(fp, "<UVList Size=\"%d\">\r\n", pMeshAnimation->m_lstTextureCoord.size());
					for (int cUV = 0; cUV < pMeshAnimation->m_lstTextureCoord.size(); cUV++) {
						fprintf(fp, "<UV>%d %d</UV>\r\n", pMeshAnimation->m_lstTextureCoord[cUV].x, pMeshAnimation->m_lstTextureCoord[cUV].y);
					}
					fprintf(fp, "</UVList>\r\n");
				}/* end for */
				fprintf(fp, "</MeshOrTextureAnimation>\r\n");
			}/* end if */
		}

		// �ؼ�֡������Ϊ0���ҷǹ�������
		if( pMesh->m_lstAnimationPositionKey.size() > 0 && !(pMesh->m_nAnimationType & REAT_BONE) ) {
			fprintf(fp, "<MeshKeyFrame>\r\n");

			// д��ؼ�֡λ��
			fprintf(fp, "<PositionKey Size=\"%d\">\r\n", pMesh->m_lstAnimationPositionKey.size());
			for (int cPosition = 0; cPosition < pMesh->m_lstAnimationPositionKey.size(); cPosition++) {
				fprintf(fp, "<Position>%f %f %f</Position>\r\n", pMesh->m_lstAnimationPositionKey[cPosition].x, pMesh->m_lstAnimationPositionKey[cPosition].y, pMesh->m_lstAnimationPositionKey[cPosition].z);
			}
			fprintf(fp, "</PositionKey>\r\n");

			// д��ؼ�֡��ת
			fprintf(fp, "<RotateKey Size=\"%d\">\r\n", pMesh->m_lstAnimationRotateKey.size());
			for (int cRotate = 0; cRotate < pMesh->m_lstAnimationRotateKey.size(); cRotate++) {
				fprintf(fp, "<Rotate>%f %f %f %f</Rotate>\r\n", pMesh->m_lstAnimationRotateKey[cRotate].x, pMesh->m_lstAnimationRotateKey[cRotate].y, pMesh->m_lstAnimationRotateKey[cRotate].z, pMesh->m_lstAnimationRotateKey[cRotate].w);
			}
			fprintf(fp, "</RotateKey>\r\n");

			// д��ؼ�֡����
			fprintf(fp, "<ScaleKey Size=\"%d\">\r\n", pMesh->m_lstAnimationScaleKey.size());
			for (int cScale = 0; cScale < pMesh->m_lstAnimationScaleKey.size(); cScale++) {
				fprintf(fp, "<Scale>%f %f %f</Scale>\r\n", pMesh->m_lstAnimationScaleKey[cScale].x, pMesh->m_lstAnimationScaleKey[cScale].y, pMesh->m_lstAnimationScaleKey[cScale].z);
			}
			fprintf(fp, "</ScaleKey>\r\n");

			// д��ؼ�֡�ܼ���Χ
			fprintf(fp, "<VisibilityKey Size=\"%d\">\r\n", pMesh->m_lstAnimationVisibilityKey.size());
			for (int cVisibility = 0; cVisibility < pMesh->m_lstAnimationVisibilityKey.size(); cVisibility++) {
				fprintf(fp, "<Visibility>%f</Visibility>\r\n", pMesh->m_lstAnimationVisibilityKey[cVisibility]);
			}
			fprintf(fp, "</VisibilityKey>\r\n");

			fprintf(fp, "</MeshKeyFrame>\r\n");
		}

		fprintf(fp, "</MeshNode>");
	}
	fprintf(fp, "</Mesh>\r\n");

	return bBoneType;
}

// д�������Ϣ��XML�ļ�
void Red3DMaxEffectUtil2008::r3dWriteEffectBoneToXML(FILE *fp, vector< BoneData * > &lstBone, bool bBoneType) {
	// д���������
	int iBoneSize = bBoneType ? (int) lstBone.size() : 0;
	fwrite( &iBoneSize, sizeof(int), 1, fp );
	fprintf(fp, "<BoneList Size=\"%d\">", iBoneSize);
	if (iBoneSize > 0) {
		// ��������д���������
		for ( int iBone = 0; iBone < iBoneSize; ++iBone )
		{
			BoneData * pBoneData = m_vecBoneList[ iBone ];
			fprintf(fp, "<Name>%d</Name>\r\n", pBoneData->strBoneName.c_str());
			fprintf(fp, "<ParentID>%d</ParentID>\r\n", pBoneData->iParentID);

			float fSave[ 16 ];
			RHelper::Matrix3ToFloat16( pBoneData->matWorld, fSave );
			fprintf(fp, "<WorldMatrix>\r\n");
			r3dWriteFloatMatrixToXML(fp, fSave);
			fprintf(fp, "</WorldMatrix>\r\n");

			RHelper::Matrix3ToFloat16( pBoneData->matLocal, fSave );
			fprintf(fp, "<LocalMatrix>\r\n");
			r3dWriteFloatMatrixToXML(fp, fSave);
			fprintf(fp, "</LocalMatrix>\r\n");

			int iSize = (int)pBoneData->vecPositionList.size();
			
			fprintf(fp, "<PositionList Size=\"%d\">\r\n", iSize);
			for (int cPosition = 0; cPosition < iSize; cPosition++) {
				fprintf(fp, "<Position>%f %f %f</Position>\r\n", pBoneData->vecPositionList[cPosition].x, pBoneData->vecPositionList[cPosition].y, pBoneData->vecPositionList[cPosition].z);
			}
			fprintf(fp, "</PositionList>\r\n");

			fprintf(fp, "<RotationList Size=\"%d\">\r\n", iSize);
			for (int cRotation = 0; cRotation < iSize; cRotation++) {
				fprintf(fp, "<Rotation>%f %f %f %f</Rotation>\r\n", pBoneData->vecRotationList[cRotation].x, pBoneData->vecRotationList[cRotation].y, pBoneData->vecRotationList[cRotation].z, pBoneData->vecRotationList[cRotation].w);
			}
			fprintf(fp, "</RotationList>\r\n");

			fprintf(fp, "<RotationList Size=\"%d\">\r\n", iSize);
			for (int cScale = 0; cScale < iSize; cScale++) {
				fprintf(fp, "<Scale>%f %f %f</Scale>\r\n", pBoneData->vecScaleList[cScale].x, pBoneData->vecScaleList[cScale].y, pBoneData->vecScaleList[cScale].z);
			}
			fprintf(fp, "</RotationList>\r\n");
		}
	}
	fprintf(fp, "</BoneList>");
}

// д����ЧXML�ļ�
void Red3DMaxEffectUtil2008::r3dWriteEffectXML( string strFileName ) {
	FILE *fp = fopen( strFileName.c_str(), "wb" );
	if( fp == NULL )
		return;

	r3dWriteEffectTextureToXML(fp, m_lstFinalTextureName);							// д��������Ϣ
	bool b = r3dWriteEffectMeshToXML(fp, m_lstMesh);								// д��ģ����Ϣ
	r3dWriteEffectBoneToXML(fp, m_vecBoneList, b);									// д�������Ϣ
	
	fclose(fp);
	return;
}

/*! \fn SaveEffectFile
 *	\brief ����r3e�ļ�
 */
void Red3DMaxEffectUtil2008::SaveEffectFile( string strFilename )
{
	// д��XML�ļ���׺Ϊ.XME
	string strXME = strFilename;
	RHelper::ReplaceExt(strXME, "XME");
	r3dWriteEffectXML(strXME);

	FILE *fp = fopen( strFilename.c_str(), "wb" );
	if( fp == NULL )
		return;

	// д����������
	int nTextureCount = (int)m_lstFinalTextureName.size();
	fwrite( &nTextureCount, sizeof( int ), 1, fp );

	// ����д������·��
	for( int cTexture=0; cTexture < nTextureCount; cTexture++ )
	{
		int iLen = (int) m_lstFinalTextureName[ cTexture ].length();
		fwrite( &iLen, sizeof( int ), 1, fp );
		fwrite( m_lstFinalTextureName[ cTexture ].c_str(), sizeof( char ) * iLen, 1, fp );
	}

	int nMeshCount = (int)m_lstMesh.size();
	bool bBoneType = false;

	// ģ������
	fwrite( &nMeshCount, sizeof( int ), 1, fp );

	// д��ģ����Ϣ
	for( int cMesh=0; cMesh < nMeshCount; cMesh++ )
	{
		RMesh *pMesh = m_lstMesh[ cMesh ];												// ��ȡ����ģ��
		fwrite( &pMesh->m_nNode, sizeof( int ), 1, fp );								// �ڵ����ID
		fwrite( &pMesh->m_nParent, sizeof( int ), 1, fp );								// ���ڵ��ID
		int iMeshNameLen = (int) strlen( pMesh->m_szName );								
		fwrite( &iMeshNameLen, sizeof( int ), 1, fp );									// ��������Ƶĳ���
		fwrite( &pMesh->m_szName, sizeof( char ) * iMeshNameLen, 1, fp );				// ���������

		// д��ģ�Ͷ����������󣬱��ؾ����Լ����������
		float fSave[ 16 ];
		RHelper::Matrix3ToFloat16( pMesh->m_matWorld, fSave );
		fwrite( fSave, sizeof( float ) * 16, 1, fp );
		RHelper::Matrix3ToFloat16( pMesh->m_matLocal, fSave );
		fwrite( fSave, sizeof( float ) * 16, 1, fp );
		RHelper::Matrix3ToFloat16( pMesh->m_matInvWorld, fSave );
		fwrite( fSave, sizeof( float ) * 16, 1, fp );
		
		// д�붥���������������ݣ���������ͣ���������
		fwrite( &pMesh->m_nVertexCount, sizeof( int ), 1, fp );
		fwrite( &pMesh->m_nIndexCount, sizeof( int ), 1, fp );
		fwrite( &pMesh->m_nBillboardType, sizeof( int ), 1, fp );
		fwrite( &pMesh->m_nAnimationType, sizeof( int ), 1, fp );

		// ȷ������������
		if( pMesh->m_nAnimationType & REAT_MESH || pMesh->m_nAnimationType & REAT_TEXTURE )
			pMesh->m_bMeshAnimation = true;
		else
			pMesh->m_bMeshAnimation = false;

		fwrite( &pMesh->m_bMeshAnimation, sizeof( bool ), 1, fp );
		int nTrack = (int)pMesh->m_lstAnimationPositionKey.size();			// �ؼ�֡֡������

		if( nTrack == 0 )
		{
			MessageBoxA( NULL, "�ؼ�֡Ϊ��.", 0, 0 );
		}
		// д��ؼ�֡����
		fwrite( &nTrack, sizeof( int ), 1, fp );

		int nPrimitive = (int)pMesh->m_lstTextureID.size();					// ÿ��ͼԪ��Ӧһ������

		// д��ͼԪ��Ϣ
		fwrite( &nPrimitive, sizeof( int ), 1, fp );

		fwrite( &pMesh->m_lstTextureID[ 0 ], sizeof( int ) * nPrimitive, 1, fp );

		fwrite( &pMesh->m_lstPosition[ 0 ], sizeof( Point3 ) * pMesh->m_lstPosition.size(), 1, fp );
		fwrite( &pMesh->m_lstTextureCoord[ 0 ], sizeof( Point2 ) * pMesh->m_lstTextureCoord.size(), 1, fp );
		fwrite( &pMesh->m_lstColor[ 0 ], sizeof( color ) * pMesh->m_lstColor.size(), 1, fp );
		fwrite( &pMesh->m_lstNormal[ 0 ], sizeof( color ) * pMesh->m_lstNormal.size(), 1, fp );
		fwrite( &pMesh->m_lstTangentU[ 0 ], sizeof( color ) * pMesh->m_lstTangentU.size(), 1, fp );
		fwrite( &pMesh->m_lstTangentV[ 0 ], sizeof( color ) * pMesh->m_lstTangentV.size(), 1, fp );
		fwrite( &pMesh->m_lstIndex[ 0 ], sizeof( WORD ) * pMesh->m_lstIndex.size(), 1, fp );

		// ��������
		if ( pMesh->m_nAnimationType & REAT_BONE )
		{
			bBoneType = true;
			vector< DWORD > vecBoneIndex;
			vector< DWORD > vecBoneWeight;

			vector< int > vecBoneTable;

			// ��ȡȨ���е�����
			int iWeightSize = (int) pMesh->m_vecWeightList.size();

			// ����Ȩ��
			for ( int iWeight = 0; iWeight < iWeightSize; ++iWeight )
			{
				unsigned char nIndex[ 4 ];
				unsigned char nWeight[ 4 ];

				// ��ȡһ��Ȩ��
				RWeight & kWeight = pMesh->m_vecWeightList[ iWeight ];

				// һ��ģ�Ͷ�����4��������Ӱ��
				int iBone = 0;
				for ( ; iBone < 4; ++iBone )
				{
					// ��ǰ
					if ( iBone < (int) kWeight.vecBoneIndex.size() )
					{
						bool bFind = false;
						int iTable = 0;
						for ( ; iTable < (int) vecBoneTable.size(); ++iTable )
						{
							if ( vecBoneTable[ iTable ] == kWeight.vecBoneIndex[ iBone ]  )
							{
								bFind = true;
								break;
							}
						}

						if ( bFind == false )
						{
							vecBoneTable.push_back( kWeight.vecBoneIndex[ iBone ] );
							iTable = (int)vecBoneTable.size() - 1;
						}

						nIndex[ iBone ] = iTable;
						nWeight[ iBone ] = (unsigned char)(kWeight.vecWeight[ iBone ] * 255.0f);
					}
					else
					{
						nIndex[ iBone ] = 0;
						nWeight[ iBone ] = 0;
					}
				}

				vecBoneIndex.push_back( *((DWORD *) nIndex) );
				vecBoneWeight.push_back( *((DWORD *) nWeight) );
			}

			// д�������
			int iBoneTableSize = (int) vecBoneTable.size();
			fwrite( &iBoneTableSize, sizeof(int), 1, fp );

			if ( iBoneTableSize > 0 )
			{
				fwrite( &vecBoneTable[ 0 ], sizeof(int), iBoneTableSize, fp );
				fwrite( &vecBoneIndex[ 0 ], sizeof(DWORD), iWeightSize, fp );
				fwrite( &vecBoneWeight[ 0 ], sizeof(DWORD), iWeightSize, fp );
			}
		}
		// ���ؼ�֡����
		else if( pMesh->m_nAnimationType & REAT_MESH || pMesh->m_nAnimationType & REAT_TEXTURE )
		{
			// �ؼ�֡������
			int nAnimationMeshTrack = (int)pMesh->m_lstMeshAnimation.size();
			fwrite( &nAnimationMeshTrack, sizeof( int ), 1, fp );

			RMeshAnimation* pMeshAnimation;
			for( int cTrack=0; cTrack < nAnimationMeshTrack; cTrack++ )
			{
				pMeshAnimation = pMesh->m_lstMeshAnimation[ cTrack ];

				// �ؼ�֡������֡��
				int iAniSize = (int) pMeshAnimation->m_lstPosition.size();
				if ( iAniSize == 0 )
				{
					pMeshAnimation->m_lstPosition.resize( pMesh->m_nVertexCount );
					pMeshAnimation->m_lstColor.resize( pMesh->m_nVertexCount );
					pMeshAnimation->m_lstNormal.resize( pMesh->m_nVertexCount );
					pMeshAnimation->m_lstTangentU.resize( pMesh->m_nVertexCount );
					pMeshAnimation->m_lstTangentV.resize( pMesh->m_nVertexCount );
					pMeshAnimation->m_lstTextureCoord.resize( pMesh->m_nVertexCount );
				}
				// �ؼ�֡���붥����������ͬ
				else if ( iAniSize < pMesh->m_nVertexCount )
				{
					int iCount = pMesh->m_nVertexCount - iAniSize;
					for ( int iAdd = 0; iAdd < iCount; ++iAdd  )
					{
						pMeshAnimation->m_lstPosition.push_back( Point3(0,0,0) );
						pMeshAnimation->m_lstColor.push_back( color(0,0,0,0) );
						pMeshAnimation->m_lstNormal.push_back( color(0,0,0,0) );
						pMeshAnimation->m_lstTangentU.push_back( color(0,0,0,0) );
						pMeshAnimation->m_lstTangentV.push_back( color(0,0,0,0) );
						pMeshAnimation->m_lstTextureCoord.push_back( Point2(0.0f,0.0f) );
					}
				}

				fwrite( &pMeshAnimation->m_lstPosition[ 0 ], sizeof( Point3 ) * pMeshAnimation->m_lstPosition.size(), 1, fp );
				fwrite( &pMeshAnimation->m_lstColor[ 0 ], sizeof( color ) * pMeshAnimation->m_lstColor.size(), 1, fp );				
				fwrite( &pMeshAnimation->m_lstNormal[ 0 ], sizeof( color ) * pMeshAnimation->m_lstNormal.size(), 1, fp );
				fwrite( &pMeshAnimation->m_lstTangentU[ 0 ], sizeof( color ) * pMeshAnimation->m_lstTangentU.size(), 1, fp );
				fwrite( &pMeshAnimation->m_lstTangentV[ 0 ], sizeof( color ) * pMeshAnimation->m_lstTangentV.size(), 1, fp );
				fwrite( &pMeshAnimation->m_lstTextureCoord[ 0 ], sizeof( Point2 ) * pMeshAnimation->m_lstTextureCoord.size(), 1, fp );
			}
		}

		if( pMesh->m_lstAnimationPositionKey.size() > 0 && !(pMesh->m_nAnimationType & REAT_BONE) )
		{		
			fwrite( &pMesh->m_lstAnimationPositionKey[ 0 ], sizeof( Point3 ) * pMesh->m_lstAnimationPositionKey.size(), 1, fp );
			fwrite( &pMesh->m_lstAnimationRotateKey[ 0 ], sizeof( Point4 ) * pMesh->m_lstAnimationRotateKey.size(), 1, fp );
			fwrite( &pMesh->m_lstAnimationScaleKey[ 0 ], sizeof( Point3 ) * pMesh->m_lstAnimationScaleKey.size(), 1, fp );
			fwrite( &pMesh->m_lstAnimationVisibilityKey[ 0 ], sizeof( float ) * pMesh->m_lstAnimationVisibilityKey.size(), 1, fp );
		}
	}

	// д���������
	int iBoneSize = bBoneType ? (int) m_vecBoneList.size() : 0;
	fwrite( &iBoneSize, sizeof(int), 1, fp );
	// ��������д���������
	for ( int iBone = 0; iBone < iBoneSize; ++iBone )
	{
		BoneData * pBoneData = m_vecBoneList[ iBone ];
		int iLen = (int) pBoneData->strBoneName.length();
		fwrite( &iLen, sizeof(int), 1, fp );
		fwrite( pBoneData->strBoneName.c_str(), sizeof(char) * iLen, 1, fp );

		fwrite( &pBoneData->iParentID, sizeof(int), 1, fp );
		float fSave[ 16 ];
		RHelper::Matrix3ToFloat16( pBoneData->matWorld, fSave );
		fwrite( fSave, sizeof( float ) * 16, 1, fp );
		RHelper::Matrix3ToFloat16( pBoneData->matLocal, fSave );
		fwrite( fSave, sizeof( float ) * 16, 1, fp );
		
		int iSize = (int) pBoneData->vecPositionList.size();
		fwrite( &iSize, sizeof(int), 1, fp );
		fwrite( &pBoneData->vecPositionList[ 0 ], sizeof(Point3), iSize, fp );
		fwrite( &pBoneData->vecRotationList[ 0 ], sizeof(Point4), iSize, fp );
		fwrite( &pBoneData->vecScaleList[ 0 ], sizeof(Point3), iSize, fp );
	}

	fclose( fp );

	MessageBoxA( NULL, _T("Export Complete!"), _T("Effect Mesh"), MB_OK );
}

string Red3DMaxEffectUtil2008::GetSaveFileNamePath(void)
{
	char szFileName[ MAX_PATH ] = "";
	char szFileTitle[ MAX_PATH ] = "";
	OPENFILENAME	ofn;
	ZeroMemory( &ofn, sizeof( OPENFILENAME ) );
	ofn.lStructSize		= sizeof( OPENFILENAME );
	ofn.hwndOwner		= hPanel;
	ofn.lpstrFile		= szFileName;
	ofn.lpstrFileTitle	= szFileTitle;
	ofn.nMaxFile		= MAX_PATH;
	ofn.nMaxFileTitle	= MAX_PATH;
	ofn.lpstrFilter	= "Red3D Effect Mesh File(*.r3d_effect_mesh)\0*.r3d_effect_mesh\0";
	ofn.lpstrDefExt	= "r3d_effect_mesh";
    if( GetSaveFileName( &ofn ) != 0 )
	{
		return szFileName;
	}
	return "";
}

/*!	\fn SetVertexWeightList
 *	\param vecWeightList ���Ȩ���б�
 *	\param pNode ��ǰ�Ľڵ����
 *	\brief ���ö���Ȩ���б�
 */
int Red3DMaxEffectUtil2008::SetVertexWeightList( vector< RWeight > & vecWeightList, INode * pNode )
{
	INT i, j;

	// ��ȡ����ģ���޸���
	Modifier* pMod = GetPhysiqueMod(pNode);
	if( pMod )
	{
		// ��ȡ�����޸����ӿ�ָ��
		IPhysiqueExport* pPhyInterface = (IPhysiqueExport*)pMod->GetInterface(I_PHYINTERFACE);
		if( pPhyInterface == NULL )
			return FALSE;

		// ��ȡ�����޸���������
		IPhyContextExport *pContextInterface = (IPhyContextExport*)pPhyInterface->GetContextInterface(pNode);
		if(pContextInterface == NULL)
			return FALSE;
		pContextInterface->ConvertToRigid(TRUE);
		pContextInterface->AllowBlending(TRUE);

		// ������NODE�����нڵ�
		INT iVtxCount = pContextInterface->GetNumberVertices();
		for ( i = 0; i < iVtxCount; i++ )
		{
			// ��ȡÿ�������������Ϣ
			IPhyVertexExport *pVtxInterface = (IPhyVertexExport*)pContextInterface->GetVertexInterface(i);
			if ( pVtxInterface == NULL )
				return FALSE;//continue;

			// ��ȡ�������
			INT iVtxType = pVtxInterface->GetVertexType();

			// ����ǹ���
			// �Ӵ������������ֹ�����ʽ,һ���ǿɽ����ںϲ�����,һ���ǲ����Ե�

			RWeight VertexWeight;
			if ( iVtxType == RIGID_TYPE )	// NON BLEND TYPE
			{
				// ��ȡ�����ڵ�
				INode *pBoneNode = ((IPhyRigidVertex*)pVtxInterface)->GetNode();								// ��ȡ��ǰ�����ܵ��Ĺ���Ӱ��
				if(pBoneNode == NULL)
					return FALSE;

				// ����ID
				INT iBoneID = GetBoneID( pBoneNode );
				if ( iBoneID < 0 )
				{
					char szMsg[100];
					sprintf(szMsg, "%s������ID����!", pNode->GetName(), i);
					MessageBoxA( NULL, szMsg, "Red3DMaxEffectUtil2008", MB_OK | MB_ICONERROR );
					return FALSE;
				}

				// ѹ�����������,����,Ĭ��Ȩ��
				VertexWeight.vecBoneName.push_back( pBoneNode->GetName() );
				VertexWeight.vecBoneIndex.push_back( iBoneID );
				VertexWeight.vecWeight.push_back( 1.0f );

				// ѹ��Ȩ�ض���
				vecWeightList.push_back( VertexWeight );
			}
			else if ( iVtxType == RIGID_BLENDED_TYPE )	// BLEND TYPE
			{
				// ���ںϵĸ��嶥��
				IPhyBlendedRigidVertex * pVtxBlendedInterface = (IPhyBlendedRigidVertex *) pVtxInterface;		// ��ȡ��ǰ�����ܵ��Ĺ���Ӱ��

				// ��������
				INT iBoneCount = pVtxBlendedInterface->GetNumberNodes();
				for ( j = 0; j < iBoneCount; j++ )
				{
					INode *pBoneNode = pVtxBlendedInterface->GetNode(j);
					INT iBoneID = GetBoneID( pBoneNode );
					if(iBoneID < 0)
					{
						char szMsg[100];
						sprintf(szMsg, "%s������ID����!", pNode->GetName(), i);
						MessageBoxA( NULL, szMsg, "Red3DMaxEffectUtil2008", MB_OK | MB_ICONERROR );
						return FALSE;
					}

					bool bSame = false;

					// ����һ�������Ȩ�ؽṹ
					for ( int k = 0; k < (int) VertexWeight.vecBoneIndex.size(); ++k )
					{
						// ��������ظ�,��������Ȩ�ص���ֵ
						if ( VertexWeight.vecBoneIndex[ k ] == iBoneID )
						{
							VertexWeight.vecWeight[ k ] += pVtxBlendedInterface->GetWeight(j);
							bSame = true;
							break;
						}
					}

					// �������ͬ������
					if ( bSame == false )
					{
						VertexWeight.vecBoneName.push_back( pBoneNode->GetName() );
						VertexWeight.vecBoneIndex.push_back( iBoneID );
						VertexWeight.vecWeight.push_back( pVtxBlendedInterface->GetWeight(j) );
					}
				}

				// ѹ��һ��Ȩ��
				vecWeightList.push_back( VertexWeight );
			}
			else
			{
				// ֱ��ѹ��һ��Ȩ��
				vecWeightList.push_back( VertexWeight );
			}
		}

		// �ͷ��ڴ�
		pPhyInterface->ReleaseContextInterface(pContextInterface);
		pMod->ReleaseInterface(I_PHYINTERFACE, pPhyInterface);


		// �������ж���,һ�������Ӧһ������Ȩ��
		set<INT> BoneIndices;
		for ( i = 0; i < (int)vecWeightList.size(); i++ )
		{
			// ����һ������Ĺ���
			for ( j = 0; j < (int)vecWeightList[ i ].vecBoneIndex.size(); j++ )
			{
				INT iBoneIndex = vecWeightList[ i ].vecBoneIndex[ j ];
				BoneIndices.insert(iBoneIndex);
			}
		}

		// �����ܹ�����������
		return (int) BoneIndices.size();
	}

	// ���û�������޸���ֱ�ӷ���Ƥ���޸���
	pMod = GetSkinMod(pNode);

	if ( pMod )
	{
		// ��ȡƤ���ӿ�
		ISkin * pSkinningInterface = static_cast<ISkin*>(pMod->GetInterface(I_SKIN)); 
		if ( pSkinningInterface == NULL )
			return FALSE; 

		ISkinContextData *pContextData = pSkinningInterface->GetContextInterface( pNode );

		// ��ȡƤ���Ķ������
		INT iVtxCount = pContextData->GetNumPoints();

		// �����������
		for ( i = 0; i < iVtxCount; i++ )
		{
			// Ƥ�������Ĺ�������
			INT iBoneCount = pContextData->GetNumAssignedBones( i );

			RWeight VertexWeight;

			// �����˶���Ĺ���
			for ( int j = 0; j < iBoneCount; j++ )
			{
				// ��ȡ��������Ľڵ�
				INode * pBoneNode = pSkinningInterface->GetBone( pContextData->GetAssignedBone( i, j ) );
				INT iBoneID = GetBoneID( pBoneNode );		// ȡ������ID

				// �������IDС��0
				if ( iBoneID < 0 )
				{
					char szMsg[100];
					sprintf(szMsg, "%s������ID����!", pNode->GetName(), i);
					MessageBoxA( NULL, szMsg, "Red3DMaxEffectUtil2008", MB_OK | MB_ICONERROR );
					return FALSE;
				}

				bool bSame = false;
				for ( int k = 0; k < (int) VertexWeight.vecBoneIndex.size(); ++k )
				{
					// ���һ�������ظ�Ӱ��һ����������������Ȩ��ֵ
					if ( VertexWeight.vecBoneIndex[ k ] == iBoneID )
					{
						VertexWeight.vecWeight[ k ] += pContextData->GetBoneWeight( i, j );
						bSame = true;
						break;
					}
				}

				// ���û����ֱ��ѹ��
				if ( bSame == false )
				{
					VertexWeight.vecBoneName.push_back( pBoneNode->GetName() );
					VertexWeight.vecBoneIndex.push_back( iBoneID );
					VertexWeight.vecWeight.push_back( pContextData->GetBoneWeight( i, j ) );
				}
			}

			vecWeightList.push_back( VertexWeight );
		}				


		// �������������
		set<INT> BoneIndices;
		for ( i = 0; i < (int)vecWeightList.size(); i++ )
		{
			for ( j = 0; j < (int)vecWeightList[ i ].vecBoneIndex.size(); j++ )
			{
				INT iBoneIndex = vecWeightList[ i ].vecBoneIndex[ j ];
				BoneIndices.insert(iBoneIndex);
			}
		}

		return (int) BoneIndices.size();

	}

	return 0;
}

// ��ȡ�����޸���
Modifier* Red3DMaxEffectUtil2008::GetPhysiqueMod(INode *pNode)
{
	Object *pObj = pNode->GetObjectRef();
	if( pObj == NULL ) return NULL;

	// �ڵ������
	char szName[ 256 ];
	strcpy( szName, pNode->GetName() );

	while( pObj->SuperClassID() == GEN_DERIVOB_CLASS_ID && pObj )
	{
		// Yes -> Cast.
		IDerivedObject *pDerivedObj = static_cast<IDerivedObject*>( pObj );

		// ������ǰ����������޸���ջ
		int ModStackIndex = 0;
		while( ModStackIndex < pDerivedObj->NumModifiers() )
		{
			// ��ȡ��ȷ���޸���ջ
			Modifier* pMod = pDerivedObj->GetModifier( ModStackIndex );
			Class_ID Temp = pMod->ClassID();
			if( pMod->ClassID() == Class_ID( PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B ) )				
				return pMod;

			// ��һ���޸���ջ
			ModStackIndex++;
		}

		pObj = pDerivedObj->GetObjRef();
	}

	return NULL;
}

// ��ȡ����ID
int Red3DMaxEffectUtil2008::GetBoneID( INode * pNode )
{
	// ����������
	for ( int i = 0; i < (int) m_vecBoneList.size(); ++i )
	{
		BoneData * pBone = m_vecBoneList[ i ];
		// �ҵ���ǰ����
		if ( pBone->pNode == pNode )
			return i;
	}

	return -1;
}

// ��ȡƤ���޸���
Modifier* Red3DMaxEffectUtil2008::GetSkinMod( INode *pNode )
{
	// ��ȡ����ָ��
	Object *pObj = pNode->GetObjectRef();
	if( pObj == NULL ) return NULL;

	// �ڵ�����
	char szName[ 256 ];
	strcpy( szName, pNode->GetName() );

	while( pObj->SuperClassID() == GEN_DERIVOB_CLASS_ID && pObj )
	{
		// Yes -> Cast.
		IDerivedObject *pDerivedObj = static_cast<IDerivedObject*>( pObj );

		// �����޸���ջ
		int ModStackIndex = 0;
		while( ModStackIndex < pDerivedObj->NumModifiers() )
		{
			// ��ȡ�޸���
			Modifier* pMod = pDerivedObj->GetModifier( ModStackIndex );

			// ��ȡ�޸���ID
			Class_ID Temp = pMod->ClassID();

			// �ж��޸�����ID�Ƿ���"SKIN_CLASSID"
			if( pMod->ClassID() == SKIN_CLASSID )				
				return pMod;

			// �����޸���ջ����
			ModStackIndex++;
		}

		// ��ȡ��һ������ָ��
		pObj = pDerivedObj->GetObjRef();
	}

	return NULL;
}

// ��ǰ�ڵ��Ƿ��ǹ���
BOOL Red3DMaxEffectUtil2008::IsBone(INode* pNode)
{
	if(pNode == 0) return FALSE;

	/*
	ObjectState os = pNode->EvalWorldState(0);
	if(os.obj == 0) return FALSE;
	if(os.obj->ClassID() == Class_ID(DUMMY_CLASS_ID, 0)) return FALSE;
	if(os.obj->ClassID() == Class_ID(BONE_CLASS_ID, 0)) return TRUE;

	Control* pCtrl = pNode->GetTMController();
	if(pCtrl->ClassID() == BIPSLAVE_CONTROL_CLASS_ID ||
	pCtrl->ClassID() == BIPBODY_CONTROL_CLASS_ID) return TRUE;
	*/

	// ��ȡ�ڵ�����
	TCHAR* szName = pNode->GetName();
	if( _tcsstr( szName, _T("Nub") ) != NULL
		|| _tcsstr( szName, _T("nub") ) != NULL
		|| _tcsncmp( szName, _T("Scene Root"), sizeof( TCHAR ) * 10 ) == 0 )
	{
		return FALSE;
	}

	if( _tcsncmp( szName, _T("Bone"), sizeof( TCHAR ) * 4 ) == 0
		|| _tcsncmp( szName, _T("bone_"), sizeof( TCHAR ) * 5 ) == 0
		|| _tcsncmp( szName, _T("Bip0"), sizeof( TCHAR ) * 4 ) == 0
		|| _tcsncmp( szName, _T("bip0"), sizeof( TCHAR ) * 4 ) == 0
		|| _tcsncmp( szName, _T("B_"), sizeof( TCHAR ) * 2 ) == 0
		|| _tcsncmp( szName, _T("b_"), sizeof( TCHAR ) * 2 ) == 0
		|| _tcsncmp( szName, _T("NULL"), sizeof( TCHAR ) * 4 ) == 0 )
	{
		return TRUE;
	}

	return FALSE;
}

// �������
void Red3DMaxEffectUtil2008::ExportBone( INode *pNode )
{
	// ������ǹ���ֱ���˳�
	if ( IsBone( pNode ) == FALSE )
		return;

	// ȷ�ϵ�ǰ�ڵ�ķ��߳���
	int v1, v2, v3;	
	bool bNegScale = RHelper::TMNegParity( pNode->GetObjectTM( 0 ) );			// �˵�ķ����Ƿ��෴

	if( bNegScale )
	{
		v1 = 0; v2 = 1; v3 = 2;
	}
	else
	{
		v1 = 2; v2 = 1; v3 = 0;
	}

	BoneData * pBoneData = new BoneData;							// ��������ڴ�
	pBoneData->pNode = pNode;										// ָ�������Ľڵ�
	pBoneData->strBoneName = pNode->GetName();						// �ڵ����Ƹ�ֵ����������

	INode *pParent = pNode->GetParentNode();						// ��ȡ�ڵ�ĸ�ָ��
	INode *pRoot = ip->GetRootNode();								// ��ȡ���ڵ�
	
	// ������ڵ㲻�Ǹ��ڵ����ȡ���ڵ��ID
	// ����Ǹ��ڵ��Ǹ��ڵ��򸸽ڵ��IDΪ-1
	if( pParent != pRoot )
		pBoneData->iParentID = GetBoneID( pNode->GetParentNode() );	
	else
		pBoneData->iParentID = -1;	

	// ���ڸ������Ҳ��Ǹ��ڵ�
	if( pParent && !pParent->IsRootNode()) {
		// ʹ�õ�ǰ�ڵ�ľֲ����� * ������������ = ��ǰ�ڵ��ڸ��ռ�ľ���
		RHelper::MAXtoD3D( pNode->GetNodeTM( 0 ) * Inverse( pNode->GetParentTM( 0 ) ), pBoneData->matLocal );
	} else {
		// ������ڵ��Ǹ��ڵ���ǰ�ڵ�ľֲ�������Ȼ����������������Եľ���
		RHelper::MAXtoD3D( pNode->GetNodeTM( 0 ), pBoneData->matLocal  );
	}

	// �������
	RHelper::MAXtoD3D( pNode->GetNodeTM( 0 ), pBoneData->matWorld  );

	// ���õ�ǰ�ڵ�Ķ����ؼ�֡�б�
	SetAnimationKeyList( pBoneData, pNode );

	// ����һ������
	m_vecBoneList.push_back( pBoneData );
}

void Red3DMaxEffectUtil2008::SetAnimationKeyList( BoneData * pBoneData, INode * pNode )
{
	TimeValue	start = ip->GetAnimRange().Start();			// ������ʼ֡
	TimeValue	end = ip->GetAnimRange().End();				// �������
	int			delta = GetTicksPerFrame();					// ��ȡ֡���
	TimeValue	t;
	Matrix3		tm;
	AffineParts ap;
	Point3		pos, prevPos;
	int nFrame = 1;

	Quat qPrev;
	qPrev.Identity();

	Point3		scale;

	// ������ǰ������ÿ֡���
	for( t = start; t < end + delta; t += delta )
	{
		Matrix3 mtTM = pNode->GetNodeTM( t ) * Inverse( pNode->GetParentTM( t ) );

		RHelper::MAXtoD3D( mtTM );

		// ����ǰ�任����ֽ�Ϊ"ƽ��","��ת","����"��������
		decomp_affine( mtTM, &ap );
		pos = ap.t;						// ȡ��ƽ������

		// ��ֵ��ǰ��ƽ��λ��
		Point3 vecPosKey;
		vecPosKey.x = pos.x;
		vecPosKey.y = pos.y;
		vecPosKey.z = pos.z;

		// ѹ�뵽��ǰ������ƽ��λ�ö���
		pBoneData->vecPositionList.push_back( vecPosKey );

		//Quat q = ap.q / qPrev;
		//qPrev  = ap.q;

		Quat q = ap.q;					// ȡ����ת��Ԫ��
		// ? Ϊʲô����Ҫȡ��
		Point4 vecRotateKey;
		vecRotateKey.x = -q.x;
		vecRotateKey.y = -q.y;
		vecRotateKey.z = -q.z;
		vecRotateKey.w = q.w;

		// ѹ�뵱ǰ��ת����
		pBoneData->vecRotationList.push_back( vecRotateKey );

		// ȡ����
		scale = ap.k;
		Point3 vecScaleKey;
		vecScaleKey.x = scale.x;
		vecScaleKey.y = scale.y;
		vecScaleKey.z = scale.z;

		pBoneData->vecScaleList.push_back( vecScaleKey );

		// ��һ֡
		nFrame++;
	}
}
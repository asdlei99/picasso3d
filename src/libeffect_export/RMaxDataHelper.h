#ifndef __RMAX_EXPORTER_HELPER__H
#define __RMAX_EXPORTER_HELPER__H

// Ч����������
enum REffectRenderType 
{
	RERT_NONE		= 0,
	RERT_NORMAL,
	RERT_ADD,

	RERT_ADD_A,
	RERT_ADD_B,
	RERT_ADD_C,
	RERT_ADD_D,
	RERT_ADD_E,

	RERT_SUB_A,
	RERT_SUB_B,
	RERT_SUB_C,
	RERT_SUB_D,
	RERT_SUB_E,

	RERT_RESUB_A,
	RERT_RESUB_B,
	RERT_RESUB_C,
	RERT_RESUB_D,
	RERT_RESUB_E
};

// Ч�����������
enum REffectBillboardType
{
	REBT_NO			 = 0,
	REBT_XYZ,
	REBT_X,
	REBT_Y,
	REBT_Z,
	REBT_XY,
	REBT_XZ,
	REBT_YZ
};

// ��Ч��������
enum REffectAnimationType
{
	REAT_KEYFRAME	= 0x00000001,			/*!< �ؼ�֡ */
	REAT_TEXTURE	= 0x00000002,			/*!< ���� */
	REAT_MESH		= 0x00000004,			/*!< ����ģ�� */
	REAT_BONE		= 0x00000008,			/*!< ���� */
};

class RHelper
{
public:
	static bool					IsBone( INode *pNode );																/*!< �ǹ��� */
	static bool					IsLight( INode *pNode );															/*!< �ǹ� */
	static bool					IsMesh( INode *pNode );																/*!< ��ģ�� */
	static int					GetBoneCount( INode *pNode );														/*!< ��ȡģ�͵Ĺ������� */
	static int					GetChildBoneCount( INode *pNode );													/*!< ��ȡģ�͵��ӹ������� */
	static int					GetBoneIndex( INode *pRoot, INode *pNode );											/*!< ��ȡ�������� */
	static INode*				GetBoneNodeByIndex( INode *pRoot, int nIndex );										/*!< ��ȡ�����ڵ�ͨ������ */
	static Matrix3				GetBoneTM( INode *pNode, TimeValue t, bool bScale = true );							/*!< ��ȡ�����任���� */
	static Modifier*			GetPhysiqueMod( INode *pNode );														/*!< ��ȡ�����޸���ָ�� */
	static TriObject*			GetTriObjectFromNode( INode *pNode, TimeValue t, bool &bDeleteIt );					/*!< ��ȡͼԪ�б�ӽڵ�ָ�� */
	static bool					TMNegParity( Matrix3 &m );															/*!< ȡ����ķ� */
	static void					MAXtoD3D( Point3 &point );															/*!< ת��MAX�㵽D3D���� */
	static void					MAXtoD3D( Matrix3 &m );																/*!< ת��MAX����D3D���� */
	static void					MAXtoD3D( const Matrix3 &m, float d3dTM[ 16 ] );									/*!< ת��MAX�㵽float�������� */
	static void					MAXtoD3D( const Matrix3 &m, Matrix3 & mOut );										/*!< ת�����󵽾������� */
	static Point3 				MAXtoR3D( Point3 &point );															/*!< ת��MAX����D3D���� */
	static bool					EqualPoint3( Point3 p1, Point3 p2 );												/*!< �������Ƿ���� */
	static bool					IsMatrialTextureMapDiffuse( Class_ID cid, int subNo );								/*!< ����������ɫͼ */
	static string				GetDiffuseTextureMap( Mtl* pMtl );													/*!< ��ȡ��ɫ����ͼ */
	static string				GetFilename( string strData );														/*!< ��ȡ�ļ��� */
	static void					ReplaceExt( string & strData, char *pExt );											/*!< �滻�ļ�����׺ */

	static Point3				GetVertexNormal(Mesh* mesh, int faceNo, RVertex* rv);								/*!< ��ȡ���㷨�� */
	static int					GetBillboardType( char *szMeshName );												/*!< ��ȡ��������� */
	static int					GetAnimationType( char *szMeshName );												/*!< ��ȡ�������� */

	static void					Matrix3ToFloat16( const Matrix3 & m, float fOut[ 16 ] );							/*!< MAX����float���� */

private:
	static int					RecursiveGetBoneIndex( INode *pRoot, INode *pNodeTest, int &cBone );				/*!< �ݹ��ȡ�������� */
	static bool					BuildIter (INode* pnode, INode** const Iterator, int& currIdx );					/*!< ���� */

public:
	static string				m_strTextureName;																	/*!< ����ͼ���� */
	
	
};

class color  
{
public:
	union
	{
		struct
		{
			unsigned char b,g,r,a;
		};
		long c;
	};

	color::color( unsigned char ia, unsigned char ir, unsigned char ig, unsigned char ib ) : a( ia ), r( ir ), g( ig ), b( ib ) {}
	color( long lnColor ) : c( lnColor ) {}

	color() {};
	~color() {};
};

// �������ݵ��б�,�����ظ����
template< class T >
void AddUniqueVector( vector<T> &vecData, T pData )
{
	for ( UINT i=0; i < vecData.size(); i++ )
	{
		if ( pData == vecData[i] )
			return;
	}
	vecData.push_back( pData );
}

/*!	\fn FindVector
 *	\brief Ѱ����������������
 */
template< class T >
int FindVector( vector<T> &vecData, T pData )
{
	for ( UINT i=0; i < vecData.size(); i++ )
	{
		if ( pData == vecData[i] )
			return i;
	}
	return -1;
}

#endif

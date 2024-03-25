#include "StdAfx.h"
#include "Star.h"
# define M_PI 3.14159265358979323846  /* pi */

//-----------------------------------------------------------------------------
Adesk::UInt32 CStar::kCurrentVersionNumber =1 ;

//-----------------------------------------------------------------------------
ACRX_DXF_DEFINE_MEMBERS (
	CStar, AcDbEntity,
	AcDb::kDHL_CURRENT, AcDb::kMReleaseCurrent, 
	AcDbProxyEntity::kNoOperation, STAR,
STEP6_DBXAPP
|Product Desc:     A description for your object
|Company:          Your company name
|WEB Address:      Your company WEB site address
)

//-----------------------------------------------------------------------------
CStar::CStar () : AcDbEntity () {
	m_p3dCenter = AcGePoint3d(0, 0, 0);
}

CStar::~CStar () {
}

//-----------------------------------------------------------------------------
//----- AcDbObject protocols
//- Dwg Filing protocol
Acad::ErrorStatus CStar::dwgOutFields (AcDbDwgFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (CStar::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params

	try {
		// Write the data members
		pFiler->writeItem(m_nID);
		pFiler->writeItem(m_nNumberVertices);
		pFiler->writeItem(m_sColor);
	}
	catch (const Acad::ErrorStatus& e) {
		acutPrintf(L"\nError in Star::dwgInFields with Data Writing: %e\n", e);
	}

	return (pFiler->filerStatus());

	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus CStar::dwgInFields (AcDbDwgFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dwgInFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be read first
	Adesk::UInt32 version =0 ;
	if ( (es =pFiler->readUInt32 (&version)) != Acad::eOk )
		return (es) ;
	if ( version > CStar::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < CStar::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params
	//.....
	try {
		switch (version) {
		case 1:
			pFiler->readItem(&m_nID);
			pFiler->readItem(&m_nNumberVertices);
			pFiler->readItem(&m_sColor);
			break;
		}
	}
	catch (const Acad::ErrorStatus& e) {
		acutPrintf(L"\nError in Star::dwgInFields with Data Reading: %e\n", e);
	}

	return (pFiler->filerStatus ()) ;
}

//- Dxf Filing protocol
Acad::ErrorStatus CStar::dxfOutFields (AcDbDxfFiler *pFiler) const {
	assertReadEnabled () ;
	//----- Save parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dxfOutFields (pFiler) ;
	if ( es != Acad::eOk )
		return (es) ;
	es =pFiler->writeItem (AcDb::kDxfSubclass, _RXST("CStar")) ;
	if ( es != Acad::eOk )
		return (es) ;
	//----- Object version number needs to be saved first
	if ( (es =pFiler->writeUInt32 (kDxfInt32, CStar::kCurrentVersionNumber)) != Acad::eOk )
		return (es) ;
	//----- Output params
	//.....

	try {
		// Write out the data members
		pFiler->writeItem(AcDb::kDxfInt32, m_nID);
		pFiler->writeItem(AcDb::kDxfInt32 + 1, m_nNumberVertices);
		pFiler->writeItem(AcDb::kDxfXTextString, m_sColor);
	}
	catch (const Acad::ErrorStatus& e) {
		acutPrintf(L"\nError in Star::dxfOutFields with Data Writing: %e\n", e);
	}

	return (pFiler->filerStatus ()) ;
}

Acad::ErrorStatus CStar::dxfInFields (AcDbDxfFiler *pFiler) {
	assertWriteEnabled () ;
	//----- Read parent class information first.
	Acad::ErrorStatus es =AcDbEntity::dxfInFields (pFiler) ;
	if ( es != Acad::eOk || !pFiler->atSubclassData (_RXST("CStar")) )
		return (pFiler->filerStatus ()) ;
	//----- Object version number needs to be read first
	struct resbuf rb ;
	pFiler->readItem (&rb) ;
	if ( rb.restype != AcDb::kDxfInt32 ) {
		pFiler->pushBackItem () ;
		pFiler->setError (Acad::eInvalidDxfCode, _RXST("\nError: expected group code %d (version #)"), AcDb::kDxfInt32) ;
		return (pFiler->filerStatus ()) ;
	}
	Adesk::UInt32 version =(Adesk::UInt32)rb.resval.rlong ;
	if ( version > CStar::kCurrentVersionNumber )
		return (Acad::eMakeMeProxy) ;
	//- Uncomment the 2 following lines if your current object implementation cannot
	//- support previous version of that object.
	//if ( version < CStar::kCurrentVersionNumber )
	//	return (Acad::eMakeMeProxy) ;
	//----- Read params in non order dependant manner
	while ( es == Acad::eOk && (es =pFiler->readResBuf (&rb)) == Acad::eOk ) {
		switch (rb.restype) {
			case AcDb::kDxfInt32:
				m_nID = rb.resval.rlong;
				break;
			case AcDb::kDxfInt32 + 1:
				m_nNumberVertices = rb.resval.rlong;
				break;
			case AcDb::kDxfXTextString:
				if (m_sColor != NULL)
					free(m_sColor);
				m_sColor = _tcsdup(rb.resval.rstring);
				break;

			default:
				//----- An unrecognized group. Push it back so that the subclass can read it again.
				pFiler->pushBackItem () ;
				es =Acad::eEndOfFile ;
				break ;
		}
	}
	//----- At this point the es variable must contain eEndOfFile
	//----- - either from readResBuf() or from pushback. If not,
	//----- it indicates that an error happened and we should
	//----- return immediately.
	if ( es != Acad::eEndOfFile )
		return (Acad::eInvalidResBuf) ;

	return (pFiler->filerStatus ()) ;
}

//-----------------------------------------------------------------------------
//----- AcDbEntity protocols
Adesk::Boolean CStar::subWorldDraw (AcGiWorldDraw *mode) {
	assertReadEnabled () ;
	// Параметры звезды
	const int nPoints = m_nNumberVertices * 2;
	double radius = 100.0;
	double innerRadius = radius / 2;
	double angle = 2 * M_PI / m_nNumberVertices; // Угловой шаг между основными вершинами

	AcGePoint3dArray points;
	// TODO: нужно ли здесь использовать геттер или и так норм?
	AcGeVector3d centerOffset = AcGeVector3d(m_p3dCenter.x, m_p3dCenter.y, m_p3dCenter.z);

	for (int i = 0; i < nPoints; ++i) {
		double r = i % 2 == 0 ? radius : innerRadius; // Используем больший или меньший радиус
		double theta = i * angle / 2; // Угол для каждой точки
		double x = cos(theta) * r;
		double y = sin(theta) * r;
		// Добавляем смещение к каждой точке
		points.append(AcGePoint3d(x, y, 0.0) + centerOffset);
	}

	// Добавляем первую точку в конец массива, чтобы замкнуть фигуру
	points.append(points.first());

	// Рисуем звезду, соединяя точки полилинией
	mode->geometry().polyline(points.length(), points.asArrayPtr());

	// Текстовая информация о звезде
	// Определение положения для текста
	AcGePoint3d textPosition = m_p3dCenter + AcGeVector3d(20.0, -radius - 20.0, 0.0); // Смещаем текст вправо от центра

	// Текстовая информация о звезде
	TCHAR buffer[256];
	_stprintf(buffer, _T("Vertices: %d"), m_nNumberVertices);
	mode->geometry().text(textPosition, AcGeVector3d(0, 0, 1), AcGeVector3d(1, 0, 0), 10.0, 1.0, 0.0, buffer);

	textPosition.y -= 15.0; // Смещение вниз для следующей строки
	_stprintf(buffer, _T("ID: %d"), m_nID);
	mode->geometry().text(textPosition, AcGeVector3d(0, 0, 1), AcGeVector3d(1, 0, 0), 10.0, 1.0, 0.0, buffer);

	textPosition.y -= 15.0;
	_stprintf(buffer, _T("Center: (%.2f, %.2f, %.2f)"), m_p3dCenter.x, m_p3dCenter.y, m_p3dCenter.z);
	mode->geometry().text(textPosition, AcGeVector3d(0, 0, 1), AcGeVector3d(1, 0, 0), 10.0, 1.0, 0.0, buffer);

	textPosition.y -= 15.0;
	_stprintf(buffer, _T("Color: %s"), m_sColor);
	mode->geometry().text(textPosition, AcGeVector3d(0, 0, 1), AcGeVector3d(1, 0, 0), 10.0, 1.0, 0.0, buffer);

	return Adesk::kTrue;
}


Adesk::UInt32 CStar::subSetAttributes (AcGiDrawableTraits *traits) {
	assertReadEnabled () ;
	AcDbEntity::subSetAttributes(traits);
	if (_tcscmp(m_sColor, _T("Red")) == 0) {
		traits->setColor(1);
	}
	else if (_tcscmp(m_sColor, _T("Green")) == 0) {
		traits->setColor(3);
	}
	else if (_tcscmp(m_sColor, _T("Blue")) == 0) {
		traits->setColor(4);
	}
	return Adesk::kTrue;
}

Acad::ErrorStatus CStar::SetCenter(const AcGePoint3d p3dCenter)
{
	assertWriteEnabled();
	m_p3dCenter = p3dCenter;

	return Acad::eOk;
}

Acad::ErrorStatus CStar::GetCenter(AcGePoint3d& p3dCenter)
{
	assertReadEnabled();
	p3dCenter = m_p3dCenter;

	return Acad::eOk;
}

Acad::ErrorStatus CStar::SetID(const Adesk::Int32 nID)
{
	assertWriteEnabled();
	m_nID = nID;

	return Acad::eOk;;
}

Acad::ErrorStatus CStar::GetID(Adesk::Int32& nID)
{
	assertReadEnabled();
	nID = m_nID;

	return Acad::eOk;
}

Acad::ErrorStatus CStar::SetNumberVertices(const Adesk::Int32 nNumberVertices)
{
	assertWriteEnabled();
	m_nNumberVertices = nNumberVertices;

	return Acad::eOk;
}

Acad::ErrorStatus CStar::GetNumberVertices(Adesk::Int32& nNumberVertices)
{
	assertReadEnabled();
	nNumberVertices = m_nNumberVertices;

	return Acad::eOk;
}

Acad::ErrorStatus CStar::SetColor(const TCHAR* sColor)
{
	assertWriteEnabled();
	if (m_sColor) {
		free(m_sColor);
	}
	m_sColor = _tcsdup(sColor);

	return Acad::eOk;
}

Acad::ErrorStatus CStar::GetColor(TCHAR*& sColor)
{
	assertReadEnabled();
	sColor = m_sColor;

	return Acad::eOk;
}


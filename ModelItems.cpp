
#include "ModelItems.h"
#include "assert.h"

PortModelItem::PortModelItem( const FabricCore::DFGBinding& binding, QString portName ) : m_binding( binding )
, m_name( portName )
{
  m_cname = m_name.toStdString();
}

size_t PortModelItem::NumChildren()
{
  return 0;
}

BaseModelItem* PortModelItem::GetChild( int i )
{
  return NULL;
}

QString PortModelItem::GetName()
{
  return m_name;
}

FTL::JSONObject* PortModelItem::GetMetadata()
{
  //m_binding.getMetadata();
  return NULL;
  //m_binding.get_
    //m_exec.getPortMetadata(m_name);
}

//#include "ModelItems.h"

QVariant PortModelItem::GetValue()
{
  //FabricCore::DFGBinding binding = m_binding.bind();
  FabricCore::RTVal val = m_binding.getArgValue( m_cname.c_str() );
  if (val.isValid())
  {
    return QVariant::fromValue<FabricCore::RTVal>( val );
  }
  return QString( "|Invalid Port|" );
}

//////////////////////////////////////////////////////////////////////////

BindingModelItem::BindingModelItem( FabricCore::DFGBinding& binding ) : m_binding( binding )
{

}

BindingModelItem::~BindingModelItem()
{

}

size_t BindingModelItem::NumChildren()
{
  return m_binding.getExec().getExecPortCount();
}

BaseModelItem* BindingModelItem::GetChild( QString childName )
{
  FabricCore::DFGExec exec = m_binding.getExec();

  int numExisting = m_children.size();
  for (int i = 0; i < numExisting; i++)
  {
    if (m_children[i]->GetName() == childName)
      return m_children[i];
  }
  //  we shouldn't ever create a child that doesn't exist!
  assert( ChildIndex( childName ) >= 0 );
  PortModelItem* res = new PortModelItem( m_binding, childName );
  m_children.push_back( res );
  return res;
}

BaseModelItem* BindingModelItem::GetChild( int index )
{
  QString childName = ChildName( index );
  return GetChild( childName );
}

int BindingModelItem::ChildIndex( QString name )
{
  FabricCore::DFGExec exec = m_binding.getExec();

  int numChildren = exec.getExecPortCount();
  for (int i = 0; i < numChildren; i++)
  {
    if (exec.getExecPortName( i ) == name)
    {
      return i;
    }
  }
  return -1;
}

QString BindingModelItem::ChildName( int i )
{
  return m_binding.getExec().getExecPortName( i );
}

QString BindingModelItem::GetName()
{
  const char* title = m_binding.getExec().getTitle();
  if (title && *title != '\0')
    return title;
  return QString( "[Exec]" );
}

FTL::JSONObject* BindingModelItem::GetMetadata()
{
  return NULL;
}

QVariant BindingModelItem::GetValue()
{
  return QString( m_binding.getExec().getTitle() );
}

void BindingModelItem::argInserted( int index, const char* name, const char* type )
{
  m_children.insert( m_children.begin() + index, new PortModelItem( m_binding, name ) );
}

void BindingModelItem::argTypeChanged( int index, const char* name, const char* newType )
{
  BaseModelItem* pChild = GetChild( name );
  assert( pChild != NULL );
  if (pChild != NULL)
  {
    assert( pChild->GetName() == name );
    // TODO:  What?  Reset the QVariant...
  }
}

void BindingModelItem::argRemoved( int index, const char* name )
{
  BaseModelItem* pChild = GetChild( name );
  assert( pChild != NULL );
  if (pChild != NULL)
  {
    assert( pChild->GetName() == name );
    pChild->emitRemoved();

    delete pChild;
    m_children.erase( m_children.begin() + index );
  }
}

void BindingModelItem::onViewValueChanged( QVariant const& var, bool commit )
{
  if (commit)
  {
    QByteArray asciiArr = var.toString().toAscii();
    m_binding.getExec().setTitle( asciiArr.data() );
    //emit modelValueChanged(var);
  }
}

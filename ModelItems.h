#include <ValueEditor/DllExport.h>
#include <ValueEditor/BaseModelItem.h>
#include <ValueEditor/QVariantRTVal.h>
#include <vector>

class PortModelItem : public BaseModelItem
{
  FabricCore::DFGBinding m_binding;
  QString m_name;
  // We store a duplicated C-style version of the
  // name for accessing the non-unicode Fabric core
  std::string m_cname;

public:
  PortModelItem(const FabricCore::DFGBinding& binding, QString portName);

  // Every port has exactly 1 child - its RTValue
  size_t NumChildren();

  virtual BaseModelItem* GetChild(int i);

  virtual QString GetName();

  virtual FTL::JSONObject* GetMetadata();

  virtual QVariant GetValue();
};

//////////////////////////////////////////////////////////////////////////
// The Root-level model item for 
// 
typedef std::vector<BaseModelItem*> ChildVec;
class BindingModelItem : public BaseModelItem
{
private:

  FabricCore::DFGBinding m_binding;
  ChildVec m_children;

public:
  BindingModelItem(FabricCore::DFGBinding& binding);
  ~BindingModelItem();

  size_t NumChildren();

  virtual BaseModelItem* GetChild( QString childName );
  virtual BaseModelItem* GetChild( int index );

  virtual int ChildIndex( QString name );
  virtual QString ChildName( int i );

  virtual QString GetName();;

  virtual FTL::JSONObject* GetMetadata();

  virtual QVariant GetValue();;

  //// BindingModelItem assumes it is the root item,
  //// and index always refers to an index in it's 
  //// list of children.
  void argInserted(int index, const char* name, const char* type);

  void argTypeChanged(int index, const char* name, const char* newType);

  void argRemoved(int index, const char* name);

  virtual void onViewValueChanged(
    QVariant const& var,
    bool commit
    );
};

#pragma once
#include <qredisclient/connection.h>
#include <QRegExp>
#include <QSharedPointer>
#include <QString>
#include <QtConcurrent>

#include "treeitem.h"

namespace ConnectionsTree {

class Operations;
class AbstractNamespaceItem;
class Model;

class AbstractNamespaceItem : public TreeItem {    
 public:
  AbstractNamespaceItem(Model& model, QWeakPointer<TreeItem> parent,
                        QSharedPointer<Operations> operations, uint dbIndex,
                        QRegExp filter=QRegExp());

  virtual ~AbstractNamespaceItem() {}

  QList<QSharedPointer<TreeItem>> getAllChilds() const override;

  uint childCount(bool recursive = false) const override;

  QSharedPointer<TreeItem> child(uint row) const override;

  QWeakPointer<TreeItem> parent() const override;

  virtual void append(QSharedPointer<TreeItem> item) {
    m_childItems.append(item);
  }

  virtual void appendNamespace(QSharedPointer<AbstractNamespaceItem> item) {
    m_childNamespaces[item->getName()] = item;
    m_childItems.append(item.staticCast<TreeItem>());
  }

  virtual QSharedPointer<AbstractNamespaceItem> findChildNamespace(
      const QByteArray& name) {
    if (!m_childNamespaces.contains(name))
      return QSharedPointer<AbstractNamespaceItem>();

    return m_childNamespaces[name];
  }

  virtual void notifyModel();

  virtual QByteArray getName() const = 0;

  virtual QByteArray getFullPath() const = 0;

  virtual bool isExpanded() const override { return m_expanded; }

  virtual void setExpanded(bool v) { m_expanded = v; }

  virtual uint getDbIndex() { return m_dbIndex; }

  virtual QSharedPointer<Operations> operations() { return m_operations; }

  virtual QRegExp getFilter() const { return m_filter; }

  virtual void showLoadingError(const QString& err);

 protected:  
  virtual void clear();

 protected:
  QWeakPointer<TreeItem> m_parent;
  QSharedPointer<Operations> m_operations;
  QList<QSharedPointer<TreeItem>> m_childItems;
  QHash<QByteArray, QSharedPointer<AbstractNamespaceItem>> m_childNamespaces;
  QRegExp m_filter;
  bool m_expanded;
  uint m_dbIndex;
};
}  // namespace ConnectionsTree

#ifndef PROXY_MODEL_WITH_HEADER_MODELS
#define PROXY_MODEL_WITH_HEADER_MODELS

#include <QPointer>
#include "HierarchicalHeaderView.h"

#if QT_VERSION >= 0x050000
    #include <QSortFilterProxyModel>
    class ProxyModelWithHeaderModels: public QSortFilterProxyModel
#else
    #include <QProxyModel>
    class ProxyModelWithHeaderModels: public QProxyModel
#endif
{
		Q_OBJECT

		QPointer<QAbstractItemModel> _horizontalHeaderModel;
		QPointer<QAbstractItemModel> _verticalHeaderModel;
	public:
		ProxyModelWithHeaderModels(QObject* parent=0);

		QVariant data(const QModelIndex& index, int role=Qt::DisplayRole) const;

		void setHorizontalHeaderModel(QAbstractItemModel* model);

//		void setVerticalHeaderModel(QAbstractItemModel* model);
};

#endif

#include "ProxyModelWithHeaderModels.h"

ProxyModelWithHeaderModels::ProxyModelWithHeaderModels(QObject* parent)
#if QT_VERSION >= 0x050000
    : QSortFilterProxyModel(parent)
#else
    : QProxyModel(parent)
#endif
{}

QVariant ProxyModelWithHeaderModels::data(const QModelIndex& index, int role) const
{
	if(_horizontalHeaderModel && role==HierarchicalHeaderView::HorizontalHeaderDataRole)
	{
		QVariant v;
		v.setValue((QObject*)_horizontalHeaderModel.data());
		return v;
	}
	if(_verticalHeaderModel && role==HierarchicalHeaderView::VerticalHeaderDataRole)
	{
		QVariant v;
		v.setValue((QObject*)_verticalHeaderModel.data());
		return v;
	}
#if QT_VERSION >= 0x050000
    return QSortFilterProxyModel::data(index, role);
#else
    return QProxyModel::data(index, role);
#endif
}

void ProxyModelWithHeaderModels::setHorizontalHeaderModel(QAbstractItemModel* headerModel)
{
	_horizontalHeaderModel=headerModel;
#if QT_VERSION >= 0x050000
    int cnt=sourceModel()->columnCount();
#else
    int cnt=model()->columnCount();
#endif
	if(cnt)
		emit headerDataChanged(Qt::Horizontal, 0, cnt-1);
}

//void ProxyModelWithHeaderModels::setVerticalHeaderModel(QAbstractItemModel* headerModel)
//{
//	_verticalHeaderModel=headerModel;
//	int cnt=model()->rowCount();
//	if(cnt)
//		emit headerDataChanged(Qt::Vertical, 0, cnt-1);
//}

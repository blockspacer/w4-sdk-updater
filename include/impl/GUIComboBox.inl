template<class ForwardIter>
void ComboBox::addItems(ForwardIter first, ForwardIter last)
{
    while (first != last)
    {
        addItem(*first);
        ++first;
    }
}

template<class ForwardIter>
void ComboBox::insertItems(int32_t index, ForwardIter first, ForwardIter last)
{
    while (first != last)
    {
        insertItem(index, *first);
        ++index;
        ++first;
    }
}

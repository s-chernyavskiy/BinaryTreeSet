# Бинарное дерево поиска. Шаблоны. STL.

Три STL-совместимых контейнера для [BinarySearchTree](https://en.wikipedia.org/wiki/Binary_search_tree), реализующие различные [способы обхода дерева (in-, pre-, post-order)](https://en.wikipedia.org/wiki/Tree_traversal) через итератор.

Контейнер должен предоставляет из себя шаблон, параметризируемый типом хранимых объетов, оператором сравнения и аллокатором, а так же удовлетворяет следующим требованиям к stl - совместимым контейнерам:

  - [контейнера](https://en.cppreference.com/w/cpp/named_req/Container)
  - [контейнера с обратным итератором](https://en.cppreference.com/w/cpp/named_req/ReversibleContainer)
  - [контейнера поддерживающие аллокатор](https://en.cppreference.com/w/cpp/named_req/AllocatorAwareContainer)
  - [oбладать двунаправленным итератом](https://en.cppreference.com/w/cpp/named_req/BidirectionalIterator)

Способ обхода дерева реализован через итератор, те оператор "++" приводит к перемещению итератора к следующему элементу в дереве, согласно правилу обхода.

## NB

Все вышеуказанные требования покрыты тестами с помощью фреймворка [Google Test](http://google.github.io/googletest).

При создании этого контейнера использовалась [Tag Dispatch Idiom](https://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Tag_Dispatching)
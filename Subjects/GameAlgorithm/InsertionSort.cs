using System.Diagnostics;

namespace GameAlgorithm
{
    public class InsertionSort<T> : AbstractSort<T>
        where T : IComparable<T>
    {
        public InsertionSort(SortOrder order, IEnumerable<T> items)
            : base(order, items)
        {
        }

        /*
         * 삽입 정렬
         * 
         * 두 번째 요소부터, 끝 요소 까지 반복하며,
         *   맨 앞 까지, 자신보다 작은 값이 나올때 까지, 한 칸씩 뒤로 밀어냄.
         *   그리고, 마지막 자리에 해당 요소를 삽입해 넣음.
         */

        public override void Sort()
        {
            var sw = new Stopwatch();

            sw.Start();
            {
                for (var i = 1; i < _items.Count; i++)
                {
                    int j;
                    var item = _items[i];

                    for (j = i - 1; j >= 0; j--)
                    {
                        var previous = _items[j];

                        if (_order == SortOrder.Ascending)
                        {
                            if (previous.CompareTo(item) < 0)
                                break;
                        }
                        else if (_order == SortOrder.Descending)
                        {
                            if (previous.CompareTo(item) > 0)
                                break;
                        }
                        _items[j + 1] = previous;
                    }
                    _items[j + 1] = item;
                }
            }
            sw.Stop();
            _elapsedTime = sw.Elapsed;
        }
    }
}

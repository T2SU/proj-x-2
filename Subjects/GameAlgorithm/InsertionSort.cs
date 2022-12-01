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
                            if (previous.CompareTo(item) > 0)
                                break;
                        }
                        else if (_order == SortOrder.Descending)
                        {
                            if (previous.CompareTo(item) < 0)
                                break;
                        }
                        _items[j + 1] = item;
                    }
                    _items[j + 1] = item;
                }
            }
            sw.Stop();
            _elapsedTime = sw.Elapsed;
        }
    }
}

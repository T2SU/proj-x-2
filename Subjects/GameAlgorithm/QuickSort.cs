using System.Diagnostics;

namespace GameAlgorithm
{
    public class QuickSort<T> : AbstractSort<T>
        where T : IComparable<T>
    {
        public QuickSort(SortOrder order, IEnumerable<T> items)
            : base(order, items)
        {
        }

        /*
         * 퀵 정렬
         * 
         * : 스텝 :
         *   (전제: minIndex < maxIndex) -- 요소가 1개 이하일 경우 제외.
         *   1. 피봇을 임의로 정함. (여기서는 맨 끝 요소)
         *   2. 파티션 (로무토 분할)
         *        반복(i = minIndex .. maxIndex):
         *          _items[i]가 피봇보다 작다: Swap(++커서, i); i++;
         *          _items[i]가 피봇보다 작다: i++;
         *        Swap(커서 + 1, maxIndex)
         *        return 커서
         *   3. 분할 정복
         *        스텝(minIndex, 파티션위치 - 1)    // 재귀
         *        스텝(파티션위치 + 1, maxIndex)    // 재귀
         */

        public override void Sort()
        {
            var sw = new Stopwatch();

            sw.Start();
            {
                Step(0, _items.Count - 1);
            }
            sw.Stop();
            _elapsedTime = sw.Elapsed;
        }

        private void Step(int minIndex, int maxIndex)
        {
            if (minIndex >= maxIndex) return;

            var pivotPlace = Partition(minIndex, maxIndex);

            Step(minIndex, pivotPlace - 1);
            Step(pivotPlace + 1, maxIndex);
        }

        private int Partition(int minIndex, int maxIndex)
        {
            var pivot = _items[maxIndex];

            var cursor = minIndex - 1; // -1
            for (var i = minIndex; i < maxIndex; i++)
            {
                if (_items[i].CompareTo(pivot) < 0) // _items[cursor] < pivot
                {
                    Swap(++cursor, i);
                }
            }

            var finalPivotPlace = cursor + 1;

            Swap(finalPivotPlace, maxIndex);
            return finalPivotPlace;
        }

        private void Swap(int i, int j)
        {
            var tmp = _items[i];
            _items[i] = _items[j];
            _items[j] = tmp;
        }
    }
}

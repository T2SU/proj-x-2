using System.Diagnostics.CodeAnalysis;

namespace GameAlgorithm
{
    public abstract class AbstractSort<T>
        where T : IComparable<T>
    {
        protected readonly List<T> _items;
        protected readonly SortOrder _order;
        protected TimeSpan? _elapsedTime;

        protected AbstractSort(SortOrder order, IEnumerable<T> items)
        {
            _order = order;
            _items = new List<T>(items);
        }

        public SortOrder Order => _order;

        public IEnumerable<T> Items => _items;

        public TimeSpan Elapsed => _elapsedTime ?? default;

        public int Count => _items.Count;

        [MemberNotNull(nameof(_elapsedTime))]
        public abstract void Sort();
    }
}

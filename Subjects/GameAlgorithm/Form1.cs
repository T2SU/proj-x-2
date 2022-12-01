namespace GameAlgorithm
{
    public partial class Form1 : Form
    {
        public Form1()
        {
            InitializeComponent();
        }

        enum DataCategory
        {
            Integer,
            String,
            Single
        }

        private void button1_Click(object sender, EventArgs e)
        {
            var order = orderType.Text switch
            {
                "오름차순" => SortOrder.Ascending,
                "내림차순" => SortOrder.Descending,
                _ => throw new NotImplementedException()
            };

            if (sortAlgorithm.Text == "삽입 정렬")
            {
                switch (elementType.Text)
                {
                    case "정수":
                        {
                            var sort = new InsertionSort<int>(order, RandomIntegers());

                            sort.Sort();
                            AppendResult(sort);
                        }
                        break;
                    case "실수":
                        {
                            var sort = new InsertionSort<float>(order, RandomSingles());

                            sort.Sort();
                            AppendResult(sort);
                        }
                        break;
                }
            }
        }

        private void AppendResult<T>(AbstractSort<T> sort)
            where T : IComparable<T>
        {
            var dataType = sort.GetType().GenericTypeArguments[0] == typeof(int) ? "정수" : "실수";
            var sortType = sort.GetType().GetGenericTypeDefinition() == typeof(InsertionSort<>) ? "삽입 정렬" : "퀵 정렬";
            var time = $"{sort.Elapsed.TotalSeconds:D02}초";

            dataGridView1.Rows.Add(sort.Count.ToString(), dataType, sortType, time);
        }

        private static IEnumerable<int> RandomIntegers()
        {
            var random = new Random();
            var list = new List<int>(1000000);
            
            for (var i = 0; i < list.Capacity; i++)
            {
                list.Add(random.Next());
            }
            return list;
        }

        private static IEnumerable<float> RandomSingles()
        {
            var random = new Random();
            var list = new List<float>(1000000);

            for (var i = 0; i < list.Capacity; i++)
            {
                list.Add(random.NextSingle());
            }
            return list;
        }
    }
}
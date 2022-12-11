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
            if (!int.TryParse(textBox1.Text.Trim(), out var count))
            {
                MessageBox.Show("요소 개수는 int32 범위의 정수만 가능합니다.");
                return;
            }

            if (sortAlgorithm.Text == "삽입 정렬")
            {
                switch (elementType.Text)
                {
                    case "정수":
                        DoSort(new InsertionSort<int>(order, RandomIntegers(count)));
                        break;
                    case "실수":
                        DoSort(new InsertionSort<float>(order, RandomSingles(count)));
                        break;
                }
            }
            else if (sortAlgorithm.Text == "퀵 정렬")
            {
                switch (elementType.Text)
                {
                    case "정수":
                        DoSort(new QuickSort<int>(order, RandomIntegers(count)));
                        break;
                    case "실수":
                        DoSort(new QuickSort<float>(order, RandomSingles(count)));
                        break;
                }
            }
        }

        private async void DoSort<T>(AbstractSort<T> sort) where T : IComparable<T>
        {
            button1.Enabled = false;
            orderType.Enabled = false;
            textBox1.Enabled = false;
            sortAlgorithm.Enabled = false;
            elementType.Enabled = false;
            try
            {
                await Task.Run(() =>
                {
                    sort.Sort();
                });
                BeginInvoke(() => AppendResult(sort));
            }
            finally
            {
                button1.Enabled = true;
                orderType.Enabled = true;
                textBox1.Enabled = true;
                sortAlgorithm.Enabled = true;
                elementType.Enabled = true;
            }
        }

        private void AppendResult<T>(AbstractSort<T> sort)
            where T : IComparable<T>
        {
            var dataType = sort.GetType().GenericTypeArguments[0] == typeof(int) ? "정수" : "실수";
            var sortType = sort.GetType().GetGenericTypeDefinition() == typeof(InsertionSort<>) ? "삽입 정렬" : "퀵 정렬";
            var time = $"{sort.Elapsed.TotalSeconds:N02}초";

            if (sort.Order == SortOrder.Ascending)
            {
                var ordered = sort.Items.Zip(sort.Items.Skip(1), (a, b) => new { a, b })
                            .All(p => p.a.CompareTo(p.b) <= 0);

                if (!ordered)
                {
                    MessageBox.Show("Assert sorting failed!");
                    return;
                }
            }
            else if (sort.Order == SortOrder.Descending)
            {
                var ordered = sort.Items.Zip(sort.Items.Skip(1), (a, b) => new { a, b })
                            .All(p => p.a.CompareTo(p.b) >= 0);

                if (!ordered)
                {
                    MessageBox.Show("Assert sorting failed!");
                    return;
                }
            }

            dataGridView1.Rows.Add(sort.Count.ToString(), dataType, sortType, time);
        }

        private static IEnumerable<int> RandomIntegers(int count)
        {
            var random = new Random();
            var list = new List<int>(count);
            
            for (var i = 0; i < list.Capacity; i++)
            {
                list.Add(random.Next());
            }
            return list;
        }

        private static IEnumerable<float> RandomSingles(int count)
        {
            var random = new Random();
            var list = new List<float>(count);

            for (var i = 0; i < list.Capacity; i++)
            {
                list.Add(random.NextSingle());
            }
            return list;
        }
    }
}
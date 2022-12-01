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
                "��������" => SortOrder.Ascending,
                "��������" => SortOrder.Descending,
                _ => throw new NotImplementedException()
            };
            if (!int.TryParse(textBox1.Text.Trim(), out var count))
            {
                MessageBox.Show("��� ������ int32 ������ ������ �����մϴ�.");
                return;
            }

            if (sortAlgorithm.Text == "���� ����")
            {
                switch (elementType.Text)
                {
                    case "����":
                        DoSort(new InsertionSort<int>(order, RandomIntegers(count)));
                        break;
                    case "�Ǽ�":
                        DoSort(new InsertionSort<float>(order, RandomSingles(count)));
                        break;
                }
            }
            else if (sortAlgorithm.Text == "�� ����")
            {
                switch (elementType.Text)
                {
                    case "����":
                        DoSort(new QuickSort<int>(order, RandomIntegers(count)));
                        break;
                    case "�Ǽ�":
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
            var dataType = sort.GetType().GenericTypeArguments[0] == typeof(int) ? "����" : "�Ǽ�";
            var sortType = sort.GetType().GetGenericTypeDefinition() == typeof(InsertionSort<>) ? "���� ����" : "�� ����";
            var time = $"{sort.Elapsed.TotalSeconds:N02}��";

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
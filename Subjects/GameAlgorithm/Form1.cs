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

            if (sortAlgorithm.Text == "���� ����")
            {
                switch (elementType.Text)
                {
                    case "����":
                        {
                            var sort = new InsertionSort<int>(order, RandomIntegers());

                            sort.Sort();
                            AppendResult(sort);
                        }
                        break;
                    case "�Ǽ�":
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
            var dataType = sort.GetType().GenericTypeArguments[0] == typeof(int) ? "����" : "�Ǽ�";
            var sortType = sort.GetType().GetGenericTypeDefinition() == typeof(InsertionSort<>) ? "���� ����" : "�� ����";
            var time = $"{sort.Elapsed.TotalSeconds:D02}��";

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
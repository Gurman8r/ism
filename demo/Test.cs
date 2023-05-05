namespace Demo
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public class Test : Ism.Script
	{
		string m_name = "Test";

		private void Start()
		{
			print(m_name + "::Start");
		}

		private void Update()
		{
			print(m_name + "::Update");
		}

		public static void SayHello()
		{
			print("Hello from CSharp!");
		}
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
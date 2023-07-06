using System;

namespace Demo
{
	[Serializable]
	public sealed class Demo : Ism.Application
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		public readonly string Version	= "0.0.0.0";
		public readonly string Guid		= "demo";
		public readonly string Name		= "demo";
		public readonly string Desc		= "";
		public readonly string Icon		= "demo.ico";

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		private void OnInitialize()
		{
		}

		private void OnFinalize()
		{
		}

		private void OnEnable()
		{
		}

		private void OnDisable()
		{
		}

		private void OnProcess(float delta_time)
		{
		}

		private void OnPhysicsProcess(float delta_time)
		{
		}

		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
	}
}
using System;

namespace Toolkit
{
	[Serializable]
	public sealed class Toolkit : Ism.Application
	{
		/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

		public readonly string Version	= "0.0.0.0";
		public readonly string Guid		= "toolkit";
		public readonly string Name		= "toolkit";
		public readonly string Desc		= "";
		public readonly string Icon		= "toolkit.ico";

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
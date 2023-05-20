using System;
using System.Runtime.CompilerServices;

namespace Ism
{
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

	public class Object
	{
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		protected static extern void print(string message);
	}

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
}
using System;


class Class
{
    public void Method1()
    {
        Console.WriteLine("M1");
        Method2();
    }

    public void Method2()
    {
        Console.WriteLine("M2");
        Method3();   
    }

    public void Method3()
    {
        Console.WriteLine("M3");
        throw new Exception("Say: 'Hi, Exception!'");
        string s = null;
        s.GetHashCode();
    }
}

class Program
{
    public static void Main()
    {
        Console.WriteLine("Hello, press any key.");
        Console.ReadLine();
        try
        {
            new Class().Method1();
        }
        catch (Exception e)
        {
            Console.WriteLine(e);
        }
    }
}
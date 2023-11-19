namespace asciilibur 
{
    public class Renderer 
    {

        public int WindowWidth { get; }
        public int WindowHeight { get; }
        private readonly string blankLine;
        private readonly object mutex = new object();

        internal Renderer(int width, int height) 
        {
            WindowWidth = width;
            WindowHeight = height;

            for (int x = 0; x < width; ++x)
            {
                blankLine += " ";
            }

            Console.CursorVisible = false;
        }

        public void Draw(int x, int y, string str) 
        {
            Draw(x, y, str, Console.ForegroundColor, Console.BackgroundColor);
        }

        public void Draw(int x, int y, string str, ConsoleColor foreground, ConsoleColor background) 
        {
            lock (mutex)
            {
                if (x < 0 || y < 0 || x >= WindowWidth || y >= WindowHeight) 
                {
                    return;
                }

                Console.ForegroundColor = foreground;
                Console.BackgroundColor = background;

                string[] lines = str.Split("\n");

                int normalizedY = WindowHeight - 1 - y;


                for (int lineIdx = 0; lineIdx < lines.Length; ++lineIdx)
                {
                    int absY = normalizedY - (lines.Length - 1 - lineIdx);

                    if (absY >= WindowHeight || absY < 0) 
                    {
                        continue;
                    }

                    int maxWidth = Math.Min(WindowWidth - 1 - x, lines[lineIdx].Length);

                    Console.SetCursorPosition(x, absY);
                    Console.Write(lines[lineIdx].Substring(0, maxWidth));
                }

                Console.ResetColor();
            }
        }

        public void ClearScreen() 
        {
            lock (mutex)
            {
                Console.SetCursorPosition(0, 0);

                for (int y = 0; y < WindowHeight; ++y)
                {
                    Console.WriteLine(blankLine);
                }
            }
        }
    }
}
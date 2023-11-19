namespace asciilibur
{
    public class Input 
    {
        private Dictionary<ConsoleKey, bool> state;

        internal Input()
        {
            state = new Dictionary<ConsoleKey, bool>();
        }

        internal void PollInput()
        {
            state.Clear();

            while (Console.KeyAvailable)
            {
                ConsoleKeyInfo info = Console.ReadKey(true);
                state.Add(info.Key, true);
            }
        }

        public bool IsKeyDown(ConsoleKey key)
        {
            return state.GetValueOrDefault(key, false);
        }
    }
}
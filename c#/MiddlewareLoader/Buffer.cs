using System.Text;


namespace MiddlewareLoader
{
    public class Buffer
    {
        public byte[] Data { get; set; }
        public int MaxBuffer { get; set; }
        public int ActualBuffer { get; set; }

        /// <summary>
        /// Metodo Construtor
        /// </summary>
        /// <param name="maxBuffer"></param>
        public Buffer(int maxBuffer = 1500)
        {
            this.MaxBuffer    = maxBuffer;
            this.ActualBuffer = 0;
            this.Data = new byte[this.MaxBuffer];
        }

        /// <summary>
        /// Metodo construtor utilizando uma string para a inicialização.
        /// </summary>
        /// <param name="str"></param>
        /// <param name="maxBuffer"></param>
        public Buffer(string str, int maxBuffer = 1500)
        {
            this.MaxBuffer    = maxBuffer;
            this.ActualBuffer = str.Length;
            this.Data         = Encoding.UTF8.GetBytes(str);
            
        }

        public void SetString(string str = "", int maxBuffer = 1500)
        {
            this.MaxBuffer = maxBuffer;
            this.ActualBuffer = str.Length;
            this.Data = Encoding.UTF8.GetBytes(str);
        }

        public Buffer(byte[] Bytes,int n_bytes, int maxBuffer = 1500)
        {
            this.Data         = Bytes;
            this.ActualBuffer = n_bytes;
            this.MaxBuffer    = maxBuffer;
        }

        /// <summary>
        /// Retorna o campo Data em string;
        /// </summary>
        /// <returns></returns>
        public override string ToString()
        {
            return Encoding.UTF8.GetString(this.Data,0, this.ActualBuffer);
        }

    }
}

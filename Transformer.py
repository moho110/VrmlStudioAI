import tensorflow as tf  
from tensorflow.keras.layers import Layer, Dense, MultiHeadAttention, LayerNormalization, Embedding  
from tensorflow.keras.models import Model  
import numpy as np  
import pandas as pd  
  
# 定义位置编码  
class PositionalEncoding(Layer):  
    def __init__(self, positional_encoding_dim, max_seq_len):  
        super(PositionalEncoding, self).__init__()  
        self.positional_encoding_dim = positional_encoding_dim  
        self.max_seq_len = max_seq_len  
  
        # 创建位置编码矩阵  
        pe = np.zeros((max_seq_len, positional_encoding_dim))  
        for pos in range(max_seq_len):  
            for i in range(0, positional_encoding_dim, 2):  
                pe[pos, i] = np.sin(pos / np.power(10000, (2 * i) / positional_encoding_dim))  
                if i + 1 < positional_encoding_dim:  
                    pe[pos, i + 1] = np.cos(pos / np.power(10000, (2 * i) / positional_encoding_dim))  
        self.positional_encoding = tf.constant(pe, dtype=tf.float32)  
  
    def call(self, inputs):  
        return inputs + self.positional_encoding[:, :tf.shape(inputs)[1]]  
  
# 定义Transformer编码器层  
class TransformerEncoderLayer(Layer):  
    def __init__(self, d_model, num_heads):  
        super(TransformerEncoderLayer, self).__init__()  
  
        self.self_attn = MultiHeadAttention(num_heads=num_heads, key_dim=d_model)  
        self.ffn = tf.keras.Sequential([  
            Dense(d_model * 4, activation='relu'),  
            Dense(d_model)  
        ])  
  
        self.layernorm1 = LayerNormalization(epsilon=1e-6)  
        self.layernorm2 = LayerNormalization(epsilon=1e-6)  
        self.dropout1 = tf.keras.layers.Dropout(0.1)  
        self.dropout2 = tf.keras.layers.Dropout(0.1)  
  
    def call(self, v, training=False):  
        attn_output = self.self_attn(v, v, v)  
        attn_output = self.dropout1(attn_output, training=training)  
        out1 = self.layernorm1(v + attn_output)  
  
        ffn_output = self.ffn(out1)  
        ffn_output = self.dropout2(ffn_output, training=training)  
        return self.layernorm2(out1 + ffn_output)  
  
# 定义Transformer编码器  
class TransformerEncoder(Layer):  
    def __init__(self, num_layers, d_model, num_heads):  
        super(TransformerEncoder, self).__init__()  
  
        self.layers = [TransformerEncoderLayer(d_model, num_heads) for _ in range(num_layers)]  
  
    def call(self, v, training=False):  
        for layer in self.layers:  
            v = layer(v, training=training)  
        return v  
  
# 主函数  
def main():  
    # 设置模型参数  
    d_model = 512  
    num_layers = 6  
    num_heads = 8  
    max_seq_len = 100  
    vocab_size = 10000  
    embedding_dim = d_model  
  
    # 创建模型输入  
    input_seq = tf.keras.Input(shape=(max_seq_len,))  
      
    # 嵌入层  
    embedding_layer = Embedding(vocab_size, embedding_dim)(input_seq)  
      
    # 位置编码  
    positional_encoding = PositionalEncoding(embedding_dim, max_seq_len)# 继续主函数  
def main():  
    # ...（之前的代码）  
  
    # 将嵌入层与位置编码结合  
    encoded_input = positional_encoding(embedding_layer)  
  
    # Transformer编码器  
    transformer_encoder = TransformerEncoder(num_layers, d_model, num_heads)  
    encoded_output = transformer_encoder(encoded_input)  
  
    # 如果这是一个监督学习任务，我们需要添加输出层  
    # 例如，对于分类任务，我们可以使用softmax输出层  
    # 对于序列生成任务，我们可能需要一个解码器  
    # 这里为了示例，我们假设是分类任务  
    output_layer = Dense(vocab_size, activation='softmax')(encoded_output[:, 0])  # 只取序列的第一个token的输出  
  
    # 构建模型  
    model = Model(inputs=input_seq, outputs=output_layer)  
  
    # 编译模型  
    model.compile(optimizer='Adam', loss='sparse_categorical_crossentropy', metrics=['accuracy'])  
  
    # 准备测试数据  
    test_sequences = np.random.randint(0, vocab_size, size=(10, max_seq_len))  # 假设有10个测试序列  
    test_labels = np.random.randint(0, vocab_size, size=(10, 1))  # 假设每个序列对应一个标签  
  
    # 训练模型（这里仅做示例，实际数据需要从数据集中加载）  
    # model.fit(train_data, train_labels, epochs=10, batch_size=32)  
  
    # 保存测试数据为CSV文件  
    test_data = pd.DataFrame(test_sequences, columns=[f'word_{i}' for i in range(max_seq_len)])  
    test_data['label'] = test_labels.flatten()  
    test_data.to_csv('test_data.csv', index=False)  
  
    # 打印模型概况  
    model.summary()  
  
# 运行主函数  
if __name__ == "__main__":  
    main()
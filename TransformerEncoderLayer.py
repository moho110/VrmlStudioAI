import tensorflow as tf  
from tensorflow.keras.layers import Layer, Dense, MultiHeadAttention, LayerNormalization  
from tensorflow.keras.models import Model  
  
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
  
class TransformerEncoder(Layer):  
    def __init__(self, num_layers, d_model, num_heads):  
        super(TransformerEncoder, self).__init__()  
  
        self.layers = [TransformerEncoderLayer(d_model, num_heads) for _ in range(num_layers)]  
  
    def call(self, v, training=False):  
        for layer in self.layers:  
            v = layer(v, training=training)  
        return v  
  
# 定义模型参数  
d_model = 512  
num_layers = 6  
num_heads = 8  
  
# 创建模型  
input_seq = tf.keras.Input(shape=(None,))  
embedding = Dense(d_model)(input_seq)  
encoder_output = TransformerEncoder(num_layers, d_model, num_heads)(embedding)  
model = Model(inputs=input_seq, outputs=encoder_output)  
  
model.compile(optimizer='adam', loss='sparse_categorical_crossentropy')  
model.summary()
def rc4_mini_encrypt(key, plaintext):
 
    N = 10
    S = [i for i in range(N)]
    j = 0
    
    print(f"[*] Khởi tạo mảng S ban đầu: {S}")
    print(f"[*] Key K: {key}\n")
    
    for i in range(N):
        j = (j + S[i] + key[i % len(key)]) % N
        S[i], S[j] = S[j], S[i]
        
    print(f"[*] Mảng S sau quá trình KSA: {S}\n")
    i = 0
    j = 0
    keystream = []
    ciphertext_chars = []
    ciphertext_ascii = []
    
    for char in plaintext:
        i = (i + 1) % N
        j = (j + S[i]) % N
        
        S[i], S[j] = S[j], S[i]
        
        k = S[(S[i] + S[j]) % N]
        keystream.append(k)
        
        char_ascii = ord(char)
        cipher_val = char_ascii ^ k
        
        ciphertext_ascii.append(cipher_val)
        ciphertext_chars.append(chr(cipher_val))

    return keystream, ciphertext_ascii, ciphertext_chars

if __name__ == "__main__":
    K = [2, 4, 1, 7]
    m = "cybersecurity"
    
    print(f"--- BẮT ĐẦU MÃ HÓA RC4 MINI ---")
    print(f"Bản rõ: '{m}'")
    
    ks, ct_ascii, ct_chars = rc4_mini_encrypt(K, m)
    
    print("--- KẾT QUẢ ---")
    print(f"1. Dòng khóa (Keystream) được sinh ra: {ks}")
    print(f"2. Bản mã (Dạng mã ASCII thập phân): {ct_ascii}")
    print(f"3. Bản mã (Dạng chuỗi ký tự): {''.join(ct_chars)}")
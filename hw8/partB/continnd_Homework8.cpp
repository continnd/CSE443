int levenshteinDist(const std::string& str1, const std::string& str2) {
    int size = str1.size() > str2.size(), m[size];
    std::string copy1, copy2;
    if (str1.size() != size) {
        copy1 = std::string(str1) + std::string(size - str1.size(), ' ');
    } else if (str2.size() != size) {
        copy2 = std::string(str2) + std::string(size - str2.size(), ' ');
    }
    for (int i = 0; i < size; i++) {
        m[i][0] = copy1[i] == copy2[0];
    }
    for (int j = 0; j < size; j++) {
        m[0][j] = copy1[0] == copy2[j];
    }
    for (int i = 1; i < size; i++) {
        for (int j = 1; j < size; j++) {
            int min = std::min({m[i - 1][j - 1], m[i][j - 1], m[i - 1][j]});
            m[i][j] = min + copy1[i] == copy2[j];
        }
    }
    return m[size][size];
}

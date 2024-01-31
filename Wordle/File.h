#pragma once

/**
 * @brief Contains file utilities.
 */
namespace File {
    /**
     * @brief Reads the file content.
     * @param path The file path.
     * @return The file content.
     */
    QString readAll(const QString &path);

    /**
     * @brief Reads all lines in a file.
     * @param path The file path.
     * @return All lines as a list.
     */
    QStringList readLines(const QString &path);
}

#ifndef __FILE_SYSTEM_H__
#define __FILE_SYSTEM_H__

#include <filesystem>
#include <vector>
#include <string>

namespace Files
{

// Forward declaration
class WavFile;

/** @class File
 *  @brief Base class for various file types 
 */
class File
{
  friend class FileSystem;

public:
  virtual ~File() = default;

  std::filesystem::path get_filepath() const
  {
    return m_filepath;
  }

  std::string get_filename() const
  {
    return m_filepath.filename().string();
  }

protected:
  File(const std::filesystem::path &path): m_filepath(path) {}

  std::filesystem::path m_filepath;
};

/** @enum PathType
 *  @brief Enum to specify the type of path to filter when listing directory contents.
 */
enum class PathType
{
  Directory,
  File,
  All,
};

/** @class FileSystem
 *  @brief Singleton class for managing file system operations.
 */
class FileSystem
{
public:
  static FileSystem& instance()
  {
    static FileSystem instance;
    return instance;
  }

	std::vector<std::filesystem::path> list_directory(const std::filesystem::path &path, PathType type = PathType::All);
	std::vector<std::filesystem::path> list_wav_files_in_directory(const std::filesystem::path &path);

  /** @brief Checks if a specified path exists.
   *  @param path The path to check.
   *  @return True if the path exists, false otherwise.
   */
  inline bool path_exists(const std::filesystem::path &path) const
  {
    return std::filesystem::exists(path);
  }

  /** @brief Checks if a specified path is a file.
   *  @param path The path to check.
   *  @return True if the path is a file, false otherwise.
   */
  inline bool is_file(const std::filesystem::path& path) const
  {
    return (path_exists(path) && std::filesystem::is_regular_file(path));
  }

  /** @brief Checks if a specified path is a WAV file.
   *  @param path The path to check.
   *  @return True if the path is a WAV file, false otherwise. 
   */
  inline bool is_wav_file(const std::filesystem::path& path) const
  {
    return (path_exists(path) && std::filesystem::is_regular_file(path) && path.extension() == ".wav");
  }

  /** @brief Checks if a specified path is a directory.
   *  @param path The path to check.
   *  @return True if the path is a directory, false otherwise.
   */
  inline bool is_directory(const std::filesystem::path& path) const
  {
    return (path_exists(path) && std::filesystem::is_directory(path));
  }

  /** @brief Converts a relative path to an absolute path.
   *  @param path The path to convert.
   *  @return The absolute path.
   */
  std::filesystem::path convert_to_absolute(const std::filesystem::path &path) const
  {
    return path.is_relative() ? std::filesystem::current_path() / path.lexically_normal() : path;
  }

	void save_to_wav_file(std::vector<float> audio_buffer, const std::filesystem::path &path);
  WavFile read_wav_file(const std::filesystem::path &path);

private:
  FileSystem() = default;
  virtual ~FileSystem() = default;

  FileSystem(const FileSystem&) = delete;
  FileSystem& operator=(const FileSystem&) = delete;
};

}  // namespace Files

#endif  // __FILE_SYSTEM_H__
#include <gtest/gtest.h>
#include <iostream>

#include "filesystem.h"

using namespace Files;


TEST(FileSystemTest, PathExists)
{
  FileSystem& fs = FileSystem::instance();

  std::filesystem::path existing_path = "src";
  ASSERT_TRUE(fs.path_exists(existing_path)) << "Existing path should return true.";

  std::filesystem::path non_existing_path = "/non/existing/path";
  ASSERT_FALSE(fs.path_exists(non_existing_path)) << "Non-existing path should return false.";
}

TEST(FileSystemTest, IsFile)
{
  FileSystem& fs = FileSystem::instance();

  std::filesystem::path file_path = "./README.md";
  ASSERT_TRUE(fs.is_file(file_path)) << "Path should be recognized as a file.";

  std::filesystem::path directory_path = "src";
  ASSERT_FALSE(fs.is_file(directory_path)) << "Path should not be recognized as a file.";
}

TEST(FileSystemTest, IsWavFile)
{
  FileSystem& fs = FileSystem::instance();

  std::filesystem::path wav_file_path = "./samples/test.wav";
  ASSERT_TRUE(fs.is_wav_file(wav_file_path)) << "Path should be recognized as a WAV file.";

  std::filesystem::path non_wav_file_path = "./README.md";
  ASSERT_FALSE(fs.is_wav_file(non_wav_file_path)) << "Path should not be recognized as a WAV file.";
}

TEST(FileSystemTest, IsDirectory)
{
  FileSystem& fs = FileSystem::instance();

  std::filesystem::path directory_path = "src";
  ASSERT_TRUE(fs.is_directory(directory_path)) << "Path should be recognized as a directory.";

  std::filesystem::path file_path = "./README.md";
  ASSERT_FALSE(fs.is_directory(file_path)) << "Path should not be recognized as a directory.";
}

TEST(FileSystemTest, ListDirectory)
{
  FileSystem& fs = FileSystem::instance();
  std::filesystem::path path = "./";

  // Get all contents from directory
  std::vector<std::filesystem::path> contents = fs.list_directory(path);
  ASSERT_FALSE(contents.empty()) << "Directory should not be empty.";

  unsigned int contents_count = contents.size();

  std::cout << "Contents of directory: " << path.string() << std::endl;
  for (const auto& entry : contents)
  {
    std::cout << entry.string() << std::endl;
  }

  // Filter by directories
  contents = fs.list_directory(path, PathType::Directory);
  ASSERT_FALSE(contents.empty()) << "Directory should contain directories.";
  ASSERT_NE(contents.size(), contents_count) << "Contents count should not be equal after filtering by directories.";

  // Filter by files
  contents = fs.list_directory(path, PathType::File);
  ASSERT_FALSE(contents.empty()) << "Directory should contain files.";
  ASSERT_NE(contents.size(), contents_count) << "Contents count should not be equal after filtering by files.";
}

TEST(FileSystemTest, ListWavFilesInDirectory)
{
  FileSystem& fs = FileSystem::instance();

  std::filesystem::path path = "./samples";

  std::vector<std::filesystem::path> wav_files = fs.list_wav_files_in_directory(path);
  ASSERT_FALSE(wav_files.empty()) << "WAV files should not be empty.";

  std::cout << "WAV files in directory: " << path.string() << std::endl;
  for (const auto& file : wav_files)
  {
    ASSERT_TRUE(fs.is_file(file)) << file.string() << " should be a file.";
    ASSERT_EQ(file.extension(), ".wav") << file.string() << " should have .wav extension.";
    std::cout << file.string() << std::endl;
  }
}

TEST(FileSystemTest, SaveToWavFile)
{
  ASSERT_EQ(1, 0) << "This is a placeholder test for saving to a WAV file.";
}

TEST(FileSystemTest, LoadWavFile)
{
  FileSystem& fs = FileSystem::instance();

  // Get a WAV file path
  std::vector<std::filesystem::path> samples = fs.list_wav_files_in_directory("./samples");
  for (const auto& sample : samples)
  {
    std::cout << sample.string() << std::endl;
  }

  std::filesystem::path wav_file_path = "./samples/test.wav";
  ASSERT_TRUE(fs.path_exists(wav_file_path)) << "WAV file should exist.";

  // Load the WAV file
  WavFile file = fs.read_wav_file(wav_file_path);

  ASSERT_EQ(file.get_filepath(), fs.convert_to_absolute(wav_file_path)) << "Loaded WAV file path should match the original path.";

  ASSERT_TRUE(fs.path_exists(file.get_filepath())) << "Loaded file should exist.";
  ASSERT_TRUE(fs.is_wav_file(file.get_filepath())) << "Loaded file should be a WAV file.";
}
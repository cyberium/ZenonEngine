#pragma once

#ifdef ZN_FBX_SDK_ENABLE

#include <fbxsdk.h>

class ZN_API CFBXStream
	: public fbxsdk::FbxStream
{
public:
	CFBXStream(std::shared_ptr<IFile> File, FbxIOPluginRegistry* PluginRegistry);
	virtual ~CFBXStream();

	/** Query the current state of the stream. */
	virtual EState GetState() override;

	/** Open the stream.
	* \return True if successful.
	* \remark Each time the stream is open or closed, the stream position must be reset to zero. */
	virtual bool Open(void* pStreamData) override;

	/** Close the stream.
	* \return True if successful.
	* \remark Each time the stream is open or closed, the stream position must be reset to zero. */
	virtual bool Close() override;

	/** Empties the internal data of the stream.
	* \return True if successful. */
	virtual bool Flush() override;

	/** Writes a memory block.
	* \param pData Pointer to the memory block to write.
	* \param pSize Size (in bytes) of the memory block to write.
	* \return The number of bytes written in the stream. */
	virtual int Write(const void* pData, int pSize) override;

	/** Read bytes from the stream and store them in the memory block.
	* \param pData Pointer to the memory block where the read bytes are stored.
	* \param pSize Number of bytes read from the stream.
	* \return The actual number of bytes successfully read from the stream. */
	virtual int Read(void* pData, int pSize) const override;

	/** If not specified by KFbxImporter::Initialize(), the importer will ask
	* the stream to select an appropriate reader ID to associate with the stream.
	* FbxIOPluginRegistry can be used to locate id by extension or description.
	* Return -1 to allow FBX to select an appropriate default. */
	virtual int GetReaderID() const override;

	/** If not specified by KFbxExporter::Initialize(), the exporter will ask
	* the stream to select an appropriate writer ID to associate with the stream.
	* KFbxIOPluginRegistry can be used to locate id by extension or description.
	* Return -1 to allow FBX to select an appropriate default. */
	virtual int GetWriterID() const override;

	/** Adjust the current stream position.
	* \param pSeekPos Pre-defined position where offset is added (FbxFile::eBegin, FbxFile::eCurrent:, FbxFile::eEnd)
	* \param pOffset Number of bytes to offset from pSeekPos. */
	virtual void Seek(const FbxInt64& pOffset, const FbxFile::ESeekPos& pSeekPos) override;

	/** Get the current stream position.
	* \return Current number of bytes from the beginning of the stream. */
	virtual long GetPosition() const override;

	/** Set the current stream position.
	* \param pPosition Number of bytes from the beginning of the stream to seek to. */
	virtual void SetPosition(long pPosition) override;

	/** Return 0 if no errors occurred. Otherwise, return 1 to indicate
	* an error. This method will be invoked whenever FBX needs to verify
	* that the last operation succeeded. */
	virtual int GetError() const override;

	/** Clear current error condition by setting the current error value to 0. */
	virtual void ClearError() override;

private:
	std::shared_ptr<IFile> m_File;
	FbxIOPluginRegistry* m_PluginRegistry;
};

#endif

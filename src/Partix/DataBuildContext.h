#pragma once
#include <map>
#include <string>
#include <memory>
#include <Partix/UniformBuffer.h>

namespace Partix
{

class DataBuildContext
{
public:
    DataBuildContext() = default;
    ~DataBuildContext() = default;

    template<typename T>
    std::pair<T&, UniformBuffer&> GetBuildData()
    {
        auto it = m_data_objects.find(T::GetName());
        if (it != m_data_objects.end())
        {
            T& data = *(dynamic_cast<T*>(it->second.first.get()));
            UniformBuffer& buffer = *(it->second.second.get());
            return {data, buffer};
        }
        else
        {
            std::unique_ptr<DataObject> data = std::make_unique<DataObjectDerive<T>>();
            std::unique_ptr<UniformBuffer> buffer = std::make_unique<UniformBuffer>();
            DataObject* data_ptr = data.get();
            UniformBuffer* buffer_ptr = buffer.get();
            m_data_objects[T::GetName()] = {std::move(data), std::move(buffer)};
            return {*(dynamic_cast<T*>(data_ptr)), *buffer_ptr};
        }
    }

    void BindProgram(unsigned int program, unsigned int& bind_point)
    {
        for (auto& data_object : m_data_objects)
        {
            data_object.second.second->BindBlock(program, data_object.first, bind_point++);
        }
    }

private:
    struct DataObject
    {
        virtual ~DataObject() = default;
    };

    template <typename T>
    struct DataObjectDerive : public DataObject
    {
        virtual ~DataObjectDerive() = default;
        T data;
    };

    std::map<std::string, std::pair<std::unique_ptr<DataObject>, std::unique_ptr<UniformBuffer>> > m_data_objects;
};
} // namespace Partix

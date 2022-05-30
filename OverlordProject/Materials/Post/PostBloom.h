#pragma once
class PostBloom final: public PostProcessingMaterial
{
public:
	PostBloom();
	~PostBloom() override = default;
	PostBloom(const PostBloom& other) = delete;
	PostBloom(PostBloom&& other) noexcept = delete;
	PostBloom& operator=(const PostBloom& other) = delete;
	PostBloom& operator=(PostBloom&& other) noexcept = delete;

	void SetThreshold(float value);
	void SetStrength(float value);
protected:
	void Initialize(const GameContext& /*gameContext*/) override {}
};
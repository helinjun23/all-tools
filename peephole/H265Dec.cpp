#include "H265Dec.h"
#include "StreamDataTyp.h"
CH265Dec::CH265Dec(int width, int height) :m_pCodecCtx(NULL), m_frame(NULL), m_pRgbBuf(NULL), m_pFrameRGB(NULL), m_pSwsCtx(NULL)
{
	AVCodec* vcodec = avcodec_find_decoder(AV_CODEC_ID_HEVC);
	m_pCodecCtx = avcodec_alloc_context3(vcodec);
	m_pCodecCtx->thread_count = 1;

	m_pCodecCtx->width = width;
	m_pCodecCtx->height = height;

	//pPlayer->m_videoctx->pix_fmt = pPlayer->m_videoctx->codec_id == AV_CODEC_ID_MJPEG ? AV_PIX_FMT_YUVJ420P : AV_PIX_FMT_YUV420P;
	//LOGI_print(s_logger, "%p video(%dx%d), %dfps", pPlayer, pPlayer->m_videoctx->width, pPlayer->m_videoctx->height, pPlayer->m_pMediaSubsession->videoFPS());

	int iret = avcodec_open2(m_pCodecCtx, vcodec, NULL);

	m_frame = av_frame_alloc();
	m_pFrameRGB = av_frame_alloc();
}

CH265Dec::~CH265Dec()
{
	avcodec_close(m_pCodecCtx);
	avcodec_free_context(&m_pCodecCtx);

	av_frame_free(&m_frame);
	av_frame_free(&m_pFrameRGB);
	if (m_pSwsCtx != NULL)
	{
		sws_freeContext(m_pSwsCtx);
	}
	if (m_pRgbBuf != NULL)
		av_free(m_pRgbBuf);

}

int CH265Dec::GetPixFmt(void)
{
	return m_pCodecCtx == NULL ? -1 : m_pCodecCtx->codec_id;
}

CDataBuf* CH265Dec::Decode(CDataBuf* pSrc)
{
	if (NULL == pSrc)
	{
		return NULL;
	}
	AVPicture avpic;

	do {
		AVPacket* pSrcPacket = pSrc->GetPacket();
#ifdef _DEBUG
		if (m_pCodecs->codec_id == AV_CODEC_ID_H264)
			THIS_LOGT_print("do decode[%p]: pts=%lld, data=%p, size=%d, nalu.type=%d", pSrcdata, pSrcPacket->pts, pSrcPacket->data, pSrcPacket->size, (int)pSrcPacket->data[4] & 0x1f);
		else
			THIS_LOGT_print("do decode[%p]: pts=%lld, data=%p, size=%d", pSrcdata, pSrcPacket->pts, pSrcPacket->data, pSrcPacket->size);
#endif
		pSrcPacket->flags = AV_PKT_FLAG_KEY;

		int got = 0;
		int len = avcodec_decode_video2(m_pCodecCtx, m_frame, &got, pSrcPacket);
		if (len < 0)
		{
			break;
		}

		if (got > 0)
		{
			CDataBuf* pDecData = new CDataBuf(m_pCodecCtx->height * m_pCodecCtx->width * 2);

			memcpy(avpic.data, m_frame->data, sizeof(avpic.data));
			memcpy(avpic.linesize, m_frame->linesize, sizeof(avpic.linesize));
			int ret = avpicture_layout(&avpic, m_pCodecCtx->pix_fmt, m_pCodecCtx->width, m_pCodecCtx->height, pDecData->GetPacket()->data, pDecData->GetPacket()->size);

			return pDecData;

#if 0//def VIDEO_SF_DEC_TEST

			if (m_pBufferManager == NULL) m_pBufferManager = new CBufferManager(avpicture_get_size(m_pCodecs->pix_fmt, m_pCodecs->width, m_pCodecs->height));
			CScopeBuffer pYuvdata(m_pBufferManager->Pop(pSrcPacket->pts));
			memcpy(avpic.data, m_frame->data, sizeof(avpic.data));
			memcpy(avpic.linesize, m_frame->linesize, sizeof(avpic.linesize));
			int ret = avpicture_layout(&avpic, m_pCodecs->pix_fmt, m_pCodecs->width, m_pCodecs->height, pYuvdata->GetPacket()->data, pYuvdata->GetPacket()->size);
			THIS_LOGD_print("got frame[%p]: pts=%lld, data=%p, size=%d, iret=%d, cost=%lld", pYuvdata.p, pYuvdata->GetPacket()->pts, pYuvdata->GetPacket()->data, pYuvdata->GetPacket()->size, ret, cost.Get());
			return pYuvdata.Detach();
			//#else

			AVPixelFormat rgb_fmt = AV_PIX_FMT_RGBA;
			if (NULL == m_pRgbBuf)
			{
				int cache_size = avpicture_get_size(rgb_fmt, m_pCodecCtx->width, m_pCodecCtx->height);
				m_pRgbBuf = (uint8_t*)av_malloc(cache_size);

				avpicture_fill((AVPicture*)m_pFrameRGB, m_pRgbBuf, rgb_fmt, m_pCodecCtx->width, m_pCodecCtx->height);

				m_pSwsCtx = sws_getContext(m_pCodecCtx->width,
					m_pCodecCtx->height,
					m_pCodecCtx->pix_fmt,
					m_pCodecCtx->width,
					m_pCodecCtx->height,
					rgb_fmt,
					SWS_FAST_BILINEAR,
					NULL,
					NULL,
					NULL);
			}
			sws_scale(m_pSwsCtx, (const uint8_t**)m_frame->data, m_frame->linesize, 0, m_pCodecCtx->height, m_pFrameRGB->data, m_pFrameRGB->linesize);

			m_mutex.Enter();
			if (1)//m_bSnapPic)
			{

				//if (m_pBufferManager == NULL) 
				//	m_pBufferManager = new CBufferManager(avpicture_get_size(m_pCodecCtx->pix_fmt, m_pCodecCtx->width, m_pCodecCtx->height));
				CLocalBuf pYuvdata(CDataBufManager::Instance()->Pop(pSrcPacket->pts));

				avpicture_layout((AVPicture*)m_frame, m_pCodecCtx->pix_fmt, m_pCodecCtx->width, m_pCodecCtx->height, pYuvdata->GetPacket()->data, pYuvdata->GetPacket()->size);

				printf("************: fmt=%d", m_pCodecCtx->pix_fmt);
				return pYuvdata.Detach();
			}
			m_mutex.Leave();
#endif
		}
	} while (0);

	return NULL;
}

int CH265Dec::Decode(CDataBuf* pSrc, int dstFmt, uint8_t* dst)
{
	return DecRgbA(pSrc, dstFmt, dst);
}

CDataBuf* CH265Dec::DecYuv(CDataBuf* pSrc)
{
	return NULL;
}

int CH265Dec::DecRgbA(CDataBuf* pSrc, int dstFmt, uint8_t* rgbBuf)
{
	if (NULL == pSrc)
	{
		return -1;
	}
	AVPicture avpic;

	int res = -1;
	do {
		AVPacket* pSrcPacket = pSrc->GetPacket();
		if (nullptr == pSrcPacket)
		{
			break;
		}
		pSrcPacket->flags = AV_PKT_FLAG_KEY;

		int got = 0;
		int len = avcodec_decode_video2(m_pCodecCtx, m_frame, &got, pSrcPacket);
		if (len < 0)
		{
			break;
		}

		if (got > 0)
		{
			AVPixelFormat rgb_fmt = AV_PIX_FMT_RGBA;// AV_PIX_FMT_RGBA;
			if (MediaDstFmtBGRA == dstFmt)
				rgb_fmt = AV_PIX_FMT_BGRA;// AV_PIX_FMT_RGBA
			if (MediaDstFmtRGBA == dstFmt)
				rgb_fmt = AV_PIX_FMT_RGBA;// AV_PIX_FMT_RGBA

			if (NULL == m_pRgbBuf)
			{
				int cache_size = avpicture_get_size(rgb_fmt, m_pCodecCtx->width, m_pCodecCtx->height);
				m_pRgbBuf = (uint8_t*)av_malloc(cache_size);

				avpicture_fill((AVPicture*)m_pFrameRGB, m_pRgbBuf, rgb_fmt, m_pCodecCtx->width, m_pCodecCtx->height);

				m_pSwsCtx = sws_getContext(m_pCodecCtx->width,
					m_pCodecCtx->height,
					m_pCodecCtx->pix_fmt,
					m_pCodecCtx->width,
					m_pCodecCtx->height,
					rgb_fmt,
					SWS_FAST_BILINEAR,
					NULL,
					NULL,
					NULL);
			}
			//avpicture_fill((AVPicture *)m_pFrameRGB, rgbBuf, rgb_fmt, m_pCodecCtx->width, m_pCodecCtx->height);
			sws_scale(m_pSwsCtx, m_frame->data, m_frame->linesize, 0, m_pCodecCtx->height, m_pFrameRGB->data, m_pFrameRGB->linesize);
			memcpy(rgbBuf, m_pRgbBuf, m_pCodecCtx->width * 4 * m_pCodecCtx->height);
			res = 1;

		}
	} while (0);

	return res;
}

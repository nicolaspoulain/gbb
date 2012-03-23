<xsl:stylesheet xmlns:xsl="http://www.w3.org/1999/XSL/Transform" version="1.0">

  <xsl:template match="/">
    <html>
      <body>
        <xsl:apply-templates />
      </body>
    </html>
  </xsl:template>

<xsl:template match="title">
  <xsl:variable name="hierarchy" select="count(ancestor::section)"/>
  <xsl:variable name="heading">h<xsl:value-of select="$hierarchy" /></xsl:variable>

  <xsl:choose>
    <xsl:when test="$hierarchy > 6">
      <h6 class="{$heading}"><xsl:value-of select="." /></h6>
    </xsl:when>
    <xsl:otherwise>
      <xsl:element name="{$heading}">
        <xsl:value-of select="." />
      </xsl:element>
    </xsl:otherwise>
  </xsl:choose>
</xsl:template>

  <xsl:template match="para">
    <p><xsl:value-of select="." /></p>
  </xsl:template>

</xsl:stylesheet>

